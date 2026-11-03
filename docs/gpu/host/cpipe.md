# CPIPE

CPIPE is the compute command processor. Compute work does not arrive on a kernel
ring: the part runs user queues whose command rings live in client memory. The
kernel keeps one management ring for the packets that register, ring and retire
those queues. This chapter defines both rings, the management packet vocabulary,
the queue descriptor, the client stream and completion.

## Ring and ownership

| Ring | Owner | Memory | Write position | Read position |
|------|-------|--------|----------------|---------------|
| Management (HIQ) | kernel | 2 KiB GTT buffer, 512 dwords | register `0x38`, dword index | register `0x3c`, dword index |
| MCS queue | client | client buffer object, 4 KiB to 1 MiB | client index block, byte position | client index block, byte position |

The management ring is the kernel's HIQ queue. It is bound with CPIPE
immediate commands (block `0x03`) in this order: queue base (`BCQ`), length in
dwords (`CQSZ`), then the doorbell offset on LG210 only (`BCDB`); `UBCQ` and
`UBCDB` unbind it. Both cursors are cleared when it is bound. Writing a new
dword index to `0x38` hands the queued packets to the pipe. The producer reserves
space from `0x3c` and leaves the last dword unused, so full and empty are
distinguishable; a packet that would cross the end is padded with no-operation
words and the next one starts at index 0.

Queue slots and VMIDs are fixed at initialization. There are eight queue slots,
0–7, namespaced by PASID: both compute frontends see all eight, and two PASIDs
may each use slot 0. There are eight VMIDs: 0 kernel, 1–3 DRM compute, 4–7 the
KCD frontend (`/dev/kcd`). KCD admits only itself at start-up; the first DRM
compute queue moves the pipe to shared admission, and the partition does not
change. A fault the kernel cannot resolve quarantines the pipe, and a successful
reset restores the admission in force before it. The partition also appears in
the [LG200 profile](../architecture/generation-lg200.md#memory).

The client queue's ring and its 64-byte index block belong to the client. The
kernel does not write either: it validates the geometry, pins the mapping, copies
the queue descriptor into a management packet, and forwards doorbells. The two
rings never share a buffer: a client stream never enters the management ring, and
a management packet never enters a client ring. A client MUST keep both mapped
until the queue is released; an unmap or clear that overlaps the range is refused
while the queue exists.

## Management packets

The kernel writes management packets into the HIQ ring. A packet is a 32-bit
stream command word followed by payload dwords:

    SCMD32(op, cfg) = (op & 0xff) | ((cfg & 0xffffff) << 8)

Every CPIPE packet uses `cfg` 0.

| Packet | Opcode | Dwords | Payload |
|--------|-------:|-------:|---------|
| Map process | `0xe0` | 10 | PASID, page-directory base, trap addresses, debug flags |
| Map queue | `0xe2` | 15 | the 14-dword queue descriptor |
| Unmap queue | `0xe3` | 3 | filter type, filter data |
| Doorbell | `0xe4` | 3 | PASID, queue id |
| Writeback 64 | `0x11` | 5 | target address, 64-bit value |

Map process is ten dwords: command `0xe0`, PASID, a zero dword, the 64-bit
page-directory base, the 64-bit trap base address, the 64-bit trap memory address,
and a debug-flags word whose bit 0 marks a debugger process. LG200 writes the
command, the PASID and the page-directory base only; words 5–9 stay zero. The
user-queue path leaves them zero on LG210 as well, because it has no trap
addresses to pass.

An unmap packet carries a filter type and one value: 1 selects all queues, 2 the
dynamic queues, 3 the queues of the PASID in the value. A doorbell names one queue
by `(PASID, queue id)` and carries nothing else, in particular not the write
position. A writeback-64 packet stores a 64-bit value at a 64-bit address and is
the management stream's fence: the kernel stores a sentinel in the word, queues
the packet with the completion value, and waits for the word to change.

Three orderings hold:

1. Map process precedes map queue for a PASID. The firmware needs the page
   directory before it can translate the queue's addresses.
2. Every create repeats map process, including one that follows an unmap of the
   same PASID: unmap removes the mapping, and a queue that skips it stalls at its
   first doorbell.
3. Free is unmap queue with filter 3 and the PASID, then a writeback-64 fence,
   then a wait. The unmap packet has no queue id: teardown is per PASID.

## Memory queue descriptor

The queue descriptor (MQD) is 14 dwords, copied inline into the map-queue packet;
a client queue has no separate descriptor object.

| Dword | Field | Meaning |
|------:|-------|---------|
| 0–1 | Ring base | 64-bit GPU virtual address of the MCS ring. |
| 2–3 | Read index address | 64-bit address of the consumed counter. |
| 4–5 | Write index address | 64-bit address of the published counter. |
| 6 | Length | Ring length in dwords. |
| 7 | PASID | Address space of the queue. |
| 8 | Queue id | 0–7. |
| 9 | Type and active | Type in bits 3:0 (0 compute, 1 SDMA, 2 HIQ); active in bit 4. |
| 10 | Doorbell offset | 0 for a client queue; only kernel queues use the LG210 aperture. |
| 11–12 | Context-save base | Context-save area address. |
| 13 | Context-save size | Context-save area size. |

The two index fields are addresses, not values: the firmware reads the counter at
the write-index address for the end of the published stream and writes the
consumed position to the read-index address. The length is in dwords; the
counters are byte positions. A client compute queue carries `0x10` in its type
word and the HIQ queue carries `0x12`. The descriptor has no VMID field: the
firmware finds the address space through the PASID, and the VMID that PASID
executes under is bound separately, by a page-directory update, before the queue
is announced.

> UNRESOLVED: whether the CPIPE acts on the context-save fields. The KCD path
> programs them when a queue supplies an area; the client-queue path leaves them
> zero; no source shows the pipe reading them.

## User queue

A client queue is created, rung and freed with `DRM_IOCTL_LOONGGPU_USERQ`, whose
`op` is `LOONGGPU_USERQ_OP_CREATE`, `LOONGGPU_USERQ_OP_FREE` or
`LOONGGPU_USERQ_OP_DOORBELL`. `ip_type` MUST be `LOONGGPU_HW_IP_COMPUTE`; create
returns the queue id in the same argument union.

| Parameter | Rule |
|-----------|------|
| Ring size | Power of two, 4 KiB to 1 MiB (`LOONGGPU_USERQ_MIN_SIZE` to `LOONGGPU_USERQ_MAX_SIZE`). |
| Ring address | 64-byte aligned, at least 64 bytes above the start of its mapping. |
| Index block | The 64 bytes immediately below `queue_va`, in the same mapping. |
| Queue id | 0–7, assigned by the kernel. |
| Queues per file | One. |

The index block and the whole ring MUST lie inside a single mapping; create
refuses a range that spans two. The client's VM MUST already have a PASID. The
one-queue limit is interface policy, not capacity: teardown is filtered by PASID,
so a second queue on the same PASID could be retired with its sibling.

The index block is 64 bytes: the write index is a 64-bit byte position at
`queue_va - 64`, the read index is a 64-bit byte position at `queue_va - 56`
written by the device, and the remaining 48 bytes are not named by the interface.

Publishing is two steps, in this order. First write the stream into the ring at
the current position and store the new byte write index into the block; then issue
the doorbell request with the same value. The store, not the request, tells the
firmware where the stream ends. The request's `wptr` MUST be 64-byte aligned and
strictly below the ring size; the kernel validates and records it, but the
doorbell packet forwards only `(PASID, queue id)`. A request without the store
publishes nothing.

A doorbell is refused with a retryable error while the device is resetting or the
pipe is quarantined; the client MUST retry, and the queue stays valid. Free tears
the queue down by PASID, returns only after the firmware acknowledges, and then
releases the queue id and the mapping lease. A failed teardown quarantines the
pipe instead of freeing memory the firmware may still read.

## MCS stream

MCS is the micro-code stream the client writes into the ring. The ring does not
carry AQL packets: a well-formed AQL packet at the write position is ignored.
Each command is a 32-bit word with the opcode in the low byte, followed by its
payload. A state-configuration header carries the target dword index as `id >> 2`
in bits 19:8 and the payload count in bits 31:20.

One dispatch is this sequence, in order:

1. State configuration (`0x89`), id 4, three dwords: packed workgroup size, group
   aperture, private aperture. The packed word holds one 10-bit field per
   dimension, each holding size minus one; it is the only place the local size
   reaches the hardware, and without it the firmware runs one wave per workgroup.
2. State configuration (`0x89`), id `0x20`, four dwords: entry low, entry high,
   and the two resource words.
3. Zero or more state-configuration blocks from id `0x40`, selected by the kernel
   descriptor's flag word. The first advances the id with OR `0x10`; each later
   one adds 8.
4. Optional cache invalidate (`0x8b`), so the waves read clean descriptors.
5. Group (`0x87`), three dwords: the grid size divided by the group size per
   dimension. These are workgroup counts, not the global size; a global size here
   starts one workgroup per work item.
6. Wait (`0x8d`), then cache invalidate (`0x8b`, tag `0xf`, end-of-pipe 0).
7. Submit (`0x8a`) with the completion signal, then writeback and barrier
   (`0x16`).

No dispatch command is emitted on LG200; the LG210 form is a different word and
MUST NOT be used here. The stream is padded with no-operation words to a 64-byte
multiple, because the published write index MUST be 64-byte aligned. A stream is
at most 256 bytes; a client SHOULD refuse a dispatch whose stream would exceed
that, because the kernel does not inspect the stream. A client MUST NOT split a
stream across the end of the ring: the next one starts at the first slot instead.

> UNRESOLVED: whether 256 bytes is a hardware packet limit or a host bound, and
> whether the firmware would rejoin a stream that straddles the ring end.

## Completion

A client queue has no completion interrupt and no kernel fence. Completion is the
submit signal: the device decrements the 64-bit word by one per completed stream,
so a client arms it with the number of streams it will submit and polls until the
word is zero.

The CPIPE interrupt, source `0x22`, is not that signal. It carries a PASID and a
VMID, and the kernel attributes it to the frontend that owns the PASID. When that
frontend is KCD, the interrupt becomes an event delivered to the owning process;
for a client queue it carries attribution only.

The kernel also reads the index block. While the write index is ahead of the read
index and the read index does not move, the kernel MAY tear the client's queues
down after a policy interval: a published stream that the firmware never consumes
is a wedge, not idle work.

## Open questions

- The condition that makes the CPIPE raise interrupt source `0x22` for a client
  queue, and whether it ever reports packet completion.
- When the firmware updates a queue's read index: per packet, per stream, or
  lazily. The kernel's stall check assumes it moves as work is consumed.
- What a CPIPE reset does to a queue that is still mapped. The DRM frontend
  refuses doorbells during a reset and re-registers in its own teardown, but
  whether the firmware keeps or drops the descriptor is not established.
