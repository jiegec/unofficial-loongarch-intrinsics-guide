# Rings

Host work reaches a pipe either as an immediate command or through a ring. A ring
is a circular buffer of 32-bit command words in device-visible memory: the
producer appends words and advances a write cursor, the pipe consumes words and
advances a read cursor. The rings are not uniform. GPIPE, BPIPE and XDMA are
kernel-owned buffers whose cursors are host registers or writeback words; CPIPE
runs a kernel management ring beside client-owned MCS queues whose cursors live in
client memory. Word formats belong to the pipe chapters.

## Ring types

| Ring | Pipe | Producer | Write cursor | Read cursor | Interrupt source id |
|------|------|----------|--------------|-------------|---------------------|
| Graphics | GPIPE | kernel | register `0x18` | writeback word | `0x01` |
| Blit | BPIPE | kernel | register `0x30` | register `0x34` | `0x02` |
| Transfer | XDMA | kernel | register `0x20` | writeback word | `0x23` |
| Management | CPIPE | kernel | register `0x38` | register `0x3c` | `0x22` |
| MCS queue | CPIPE | client | client index block | client index block | `0x22` |
| Display, encode | DPIPE, EPIPE | — | — | — | absent on LG200 |

The first four rows are the whole kernel-ring set on LG200. The display and encode
rings belong to LG210 and later parts; their cursor registers (`0x48`/`0x4c` and
`0x50`/`0x54`) are unused here.

A ring belongs to one pipe. A submission names exactly one ring — a second,
different ring in the same submission is refused — and a submission naming the
compute engine is refused outright, because compute has no kernel ring.

The virtual-memory domain is a property of the packet stream, not of the ring. A
VM-select word (opcode `0x0f`) sets it for the packets that follow, and the
indirect-buffer control word repeats it in bits 31:24. GPIPE and BPIPE emit the
select ahead of each buffer; XDMA carries the domain in the control word alone. A
frame with no job behind it runs in domain 0, and the GPIPE and BPIPE fence tails
select domain 0 explicitly while the XDMA fence tail selects nothing. A client frame
first programs its domain's page-directory base with register writes, then selects
the domain.

The part provides eight VMIDs, and they are partitioned: 0 for the kernel, 1–3 for
the client pool, 4–7 for the compute pipe. The control word's field is eight bits
wide, so values above 7 encode, but no context exists behind them: a client MUST NOT
use one. One header comment still says that sixteen VMIDs exist; it contradicts the
two definitions that set the count to eight and the masks that partition exactly
0–7, so the comment is stale.

## Cursors

Every kernel ring cursor is a **dword index** into the ring, not a byte offset: the
write cursor is written to the pipe as a dword count and the ring is wrapped by
masking with `ring_size / 4 - 1`, so a cursor takes values
`0 .. ring_size/4 - 1`.

The write cursor is the producer's running dword count, masked at publication. The
read cursor is whatever the pipe reports: GPIPE and XDMA take it from a writeback
word, BPIPE and CPIPE read a register. The pipe is idle when the two are equal, and
their difference is its backlog — not a free-space measurement, because a lazily
written read pointer lags real consumption. The GPIPE, BPIPE and XDMA rings do not
reserve space from it; the CPIPE management ring does (see [Ring reuse](#ring-reuse)).

> UNRESOLVED: when the read-cursor writeback word that GPIPE and XDMA use is
> updated, and whether it agrees with the read-pointer register the same pipes
> expose. A driver has no evidence for treating either as exact consumption.

Frames are published atomically. Emitters write into a host-side staging buffer; at
publish the staged dwords are copied into the ring at the masked write cursor, a
barrier is executed, and only then is the new write cursor written to the pipe. A
pipe never observes a half-written frame, and a frame that overruns its reservation
is dropped at publication instead of being published short. Publishing also pads the
frame with no-operation words to the next 16-dword boundary, so every frame starts
and ends on a 64-byte boundary of the ring; a frame may still wrap, because its tail
is written at offset 0.

An MCS queue uses a different convention. Its write and read positions are 64-bit
**byte offsets** into the ring, stored in a 64-byte index block that immediately
precedes it — write position at offset 0 of the block, read position at offset 8 —
and the queue descriptor gives the firmware the addresses of the two positions. The
descriptor's length field is a separate value: `queue_size / 4`, a dword count of
the ring. A position ranges over `0 .. queue_size - 1`, and the stream starts at
byte 0 of the ring, so the wrap never touches the index block.

A well-formed producer publishes a position that is 64-byte aligned, and keeps a
stream from straddling the end of the ring by starting a fresh slot instead; that is
legal because an MCS stream is self-delimiting.

> UNRESOLVED: how the firmware wraps a byte position against the descriptor's dword
> length, and what it does with a position at or beyond `queue_size`. The index block
> is client memory: the kernel reads it to report progress but never validates the
> positions, so an out-of-range value reaches the firmware unchecked.

## Ring memory

A GPIPE, BPIPE or XDMA ring is sized from the largest frame it must hold and the
number of frames that may be outstanding:

    ring_size = roundup_pow_of_two(max_dw * 4 * submission_limit)   [bytes]

It is allocated page-aligned, filled with no-operation words before first use, and
programmed by the immediate commands that bind the queue, which take the base
address and the size as `ring_size / 4` dwords. With the shipped defaults —
256-dword frames and two outstanding frames — the ring is 2 KiB, or 512 dwords.
These are host policies, not hardware fields: the ring must be a power-of-two dword
array whose size and base the pipe is told, and it is a kernel allocation that no
client maps or addresses. The CPIPE management ring is not sized this way; it is a
fixed 2 KiB queue whose producer is throttled by its read cursor.

A client MCS queue is the exception, because its memory is the client's:

| Queue parameter | Rule |
|-----------------|------|
| Size | Power of two, 4 KiB to 1 MiB |
| Alignment | Ring start 64-byte aligned |
| Index block | 64 bytes immediately before the ring, same mapping |
| Lifetime | Mapped until the queue is released |
| Queues per open file | One |

The one-queue limit is an interface policy, not a capacity: the teardown packet
filters by PASID, so a second queue on the same PASID could be torn down with its
sibling.

## IB alignment

Each engine reports a start alignment and a size alignment in its hardware-IP query
reply:

| Engine | Reported start alignment | Reported size alignment |
|--------|-------------------------:|------------------------:|
| GPIPE | 32 B | 8 B |
| BPIPE | 32 B | 8 B |
| DPIPE, EPIPE | 32 B | 8 B |
| CPIPE | 64 B | 64 B |
| XDMA | 256 B | 8 B |

These are declarations to the client, not admission rules. What a submission is
actually held to is weaker: the start address is 4-byte aligned, the length is a
nonzero multiple of 4 bytes, and the length does not exceed `0x00ff_ffff` dwords.
That bound is an encoder bound, not a hardware one: the control word packs the dword
count in bits 23:0 and the domain in bits 31:24, so a longer buffer would overwrite
the domain field instead of asking for more commands.

The CPIPE row differs in kind. Compute is a user-queue engine: the "rings" reported
for it are queue slots, its 64-byte granularity is the MCS packet granularity, and no
indirect buffer is submitted to it.

> UNRESOLVED: whether any command processor requires the reported start alignments.
> The kernel enforces only 4-byte alignment, so a client that ignores the report
> reaches the ring; whether such a buffer executes correctly is not established.

## IB admission

An indirect buffer is admitted, not trusted. Before a submission is emitted, each
client buffer is checked against the submission's residency set and the client's page
tables, and refused if:

- its length is zero, not a multiple of 4 bytes, or too large for the 24-bit field;
- its start address is not 4-byte aligned, or is zero;
- its flag word sets a bit the interface does not implement;
- its range overflows, or no single present mapping covers all of it;
- its backing buffer is absent from the submission's residency set, or the range
  falls outside that buffer;
- the page directory this frame installs cannot translate the start address.

Zero and the page-directory check are policy: address zero is never valid in this
address space, and a buffer the frame's own page directory cannot translate would
fault the pipe at fetch time. Admission guarantees reachability and nothing about the
contents, which the kernel does not parse for any pipe on this generation.

Inside a frame each buffer costs a fixed number of ring dwords — one VM-select word
plus a 4-dword indirect-buffer word on GPIPE and BPIPE, 4 dwords in total on XDMA —
and the whole submission is refused as a unit, never split or retried.

## Doorbell

For a kernel ring the doorbell is the write-cursor register write itself: GPIPE,
BPIPE, XDMA and the CPIPE management ring advance when the new dword cursor is
written. No client register or mapping takes part.

A CPIPE MCS queue is rung differently. The client writes the new position into its
own index block, then issues a doorbell request naming the queue; that becomes a
doorbell packet carrying only the PASID and the queue id, and the firmware reads the
index block. LG200 has no doorbell aperture, so the packet is the only path; LG210
adds an aperture, and the packet is still what publishes the queue.

Publishing the index and ringing the doorbell are two steps, in that order. A
client MUST publish the index before it rings: a doorbell that arrives before the
index is visible cannot announce that work, and the work then waits for the next
doorbell. The write position carried by the request must be 64-byte aligned and
strictly below the queue size; the kernel validates it but does not forward it, so
it checks the client rather than steering the firmware. A doorbell is refused with
a retryable error while the device is resetting or the compute pipe is quarantined;
the client must retry.

## Completion

Completion is a memory write, not an interrupt. A frame is followed by a writeback
packet that stores the frame's sequence number in a 4-byte slot of the kernel
writeback buffer. The host reads that slot and completes every fence up to the value
it finds. The end-of-pipe interrupt does not say which fence completed: it tells the
host to sample the slot again, and a lost interrupt is recovered by re-sampling on a
timer. The slot, not the interrupt, is the completion record.

| Pipe | Fence tail | Dwords |
|------|------------|-------:|
| GPIPE | VM select, writeback 32, address, data, ordering, interrupt | 7 |
| BPIPE | VM select, writeback 32, address, data, interrupt | 6 |
| XDMA | writeback 32, address, data, interrupt | 5 |

The kernel's fence always requests the interrupt; a fence a client asked for may omit
it and save one dword. Only GPIPE orders the writeback with a separate ordering
packet before the interrupt. XDMA emits neither a VM-select nor an ordering packet.

A client queue has no kernel fence. The client places a writeback or writeback-64
packet of its own in the MCS stream and watches that word; the CPIPE interrupt is
delivered to whichever frontend owns the PASID and, for a client queue, carries
attribution only.

> UNRESOLVED: whether a fence writeback retires when the pipe issues it or when the
> pipe has drained the frame. The host derives the completed prefix from the word
> alone, so if the word can be written before the frame's work retires, a fence does
> not prove that the ring space or the buffers behind it are free.

> UNRESOLVED: the read-cursor update granularity of each pipe, and whether a producer
> can observe a partially consumed packet. No source states whether a read cursor
> moves per packet, per frame or lazily.

## Limits

| Limit | Value |
|-------|-------|
| Chunks per submission | 1–4096 |
| Payload per chunk | 4 MiB |
| Indirect buffer length | `0x00ff_ffff` dwords |
| Reported indirect buffers per submission | 39 GPIPE, 54 XDMA, 50 BPIPE/DPIPE/EPIPE, 0 CPIPE |
| Largest frame | 256 dwords |
| Outstanding frames per fence-driven ring | Power of two; two by default |
| Fence slots per ring | Twice the outstanding-frame limit |
| Kernel ring size | 2 KiB for every kernel ring |
| Client queue size | 4 KiB to 1 MiB, power of two |

The per-frame figure is what the hardware-IP query reports: a host framing budget, not
a hardware limit, and not every engine computes it — the 50 is a constant. What is
enforced is the frame, and one that overruns its reservation is refused when it is
published, not when it is built. The outstanding-frame limit is per ring and shared by
every client of that ring, so one busy client throttles the others.

## Ring reuse

Ring space is reclaimed by fence, not by cursor. A frame's dwords may be overwritten
once the fence that follows it has signalled, and the number of frames outstanding at
once is capped at the outstanding-frame limit. That cap is what makes the arithmetic
safe: with the cap applied at allocation and every frame no larger than the
largest-frame limit, the live window can never exceed `max_dw * outstanding` dwords,
and the ring is sized to hold at least that much. The write cursor is therefore never
more than one ring ahead of the oldest unsettled fence, and a producer that runs out
of window waits instead of overwriting unread words.

Reuse has three preconditions, and only the first is checked:

1. the frame's fence has signalled;
2. the pipe has finished reading the frame's words;
3. no buffer the frame referenced is still in use by the pipe.

The second and third follow from the first only under the drain assumption left open
under [Completion](#completion). A client that needs the stronger guarantee must wait
for a later fence or quiesce the pipe.

> UNRESOLVED: whether a command processor accepts a packet that straddles the ring
> wrap. On the GPIPE, BPIPE and XDMA rings the 16-dword frame padding avoids splitting
> a frame, not a packet, and no source states whether the fetcher rejoins a packet
> across the end of the buffer. An MCS producer sidesteps the question by starting a
> fresh slot instead.

The CPIPE management ring is the exception: its producer reserves space from the read
cursor, keeps one dword of capacity unused to distinguish full from empty, pads the
tail with no-operation words instead of letting a packet wrap, and waits for the read
cursor to advance before writing at offset 0. That protocol is cursor-based, not
fence-based.
