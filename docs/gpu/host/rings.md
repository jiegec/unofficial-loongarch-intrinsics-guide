# Rings

A ring is a circular buffer of command words in GPU memory, driven by a pair of
cursors. The producer writes command words and advances the write cursor; the
command processor consumes words and advances the read cursor. This chapter
describes the ring model that is common to the pipes. The packet vocabulary of
each pipe is in its own chapter.

## Ring types

| Ring | Pipe | Owner | VMID | Purpose |
|------|------|-------|------|---------|
| Graphics | GPIPE | DRM | 1–3 | Graphics indirect buffers. |
| Blit | BPIPE | DRM | — | Inner graphics commands and blits. |
| Compute | CPIPE | KCD/DRM; MCS ring user-owned | 4–7 | Compute queues and user queues. |
| Transfer | XDMA | DRM | — | Linear copy and fill. |
| Display | DPIPE | — | — | Not used on LG200. |
| Encode | EPIPE | — | — | Not used on LG200. |

A ring belongs to one pipe. A submission MUST NOT mix pipes on one ring.

## Cursors

The kernel ring cursors at the register offsets in [MMIO](mmio.md) are dword
offsets into the ring. The producer writes the write cursor after it has placed
complete command words; the command processor writes the read cursor after it
has consumed them. The difference between the two cursors is the amount of
queued work.

A user compute queue has a different convention: its read and write pointers are
byte offsets in the queue's index block, and the driver publishes a byte write
pointer through the doorbell. The kernel translates the queue pointer into the
CPIPE ring.

A producer MUST NOT overwrite a word that the command processor has not
consumed. A ring that is full is a flow-control condition, not an error: the
producer waits for the read cursor to advance.

## Ring memory

The ring buffer is a contiguous GPU allocation. Its size is a power of two and
is programmed in dwords for a kernel ring. The ring MUST be mapped readable and
writable by the command processor; an indirect buffer referenced by the ring
MUST additionally be mapped executable.

The ring's base address and size are programmed by the kernel through the
provisioning commands. A user-mode driver programs no kernel ring register. For
a CPIPE MCS queue, the user writes that queue's own write pointer in its index
block; the kernel installs the MQD and rings the HIQ, but does not write the
queue pointer.

## IB alignment

The indirect-buffer start address and length have per-engine alignment
requirements:

| Engine | Start alignment | Size alignment |
|--------|----------------:|---------------:|
| GPIPE | 32 B | 8 B |
| BPIPE / DPIPE / EPIPE | 32 B | 8 B |
| CPIPE | 64 B | 64 B |
| XDMA | 256 B | 8 B |

The kernel's admission check establishes only 4-byte alignment. The stricter
starts in the table are host conventions; whether the command processors require
them is UNRESOLVED. A submission whose IB violates the kernel check is rejected
before it reaches the ring.

## Doorbell

GPIPE, BPIPE and XDMA rings are advanced by the kernel through the write-cursor
register. The CPIPE has no user-visible BAR doorbell. A user compute queue is
rung through the HIQ doorbell packet (or the user-queue doorbell ioctl), which
carries the queue's PASID and queue id.

## Completion

Each pipe has an end-of-pipe interrupt source:

| Pipe | Source |
|------|-------:|
| GPIPE | `0x01` |
| BPIPE | `0x02` |
| CPIPE | `0x22` |
| XDMA | `0x23` |

A fence is a writeback packet placed after the work. The kernel records the
sequence number in a fence buffer and signals the submission's sync object. A
completion reported through an interrupt and a completion reported through the
fence word have the same meaning; the fence word is the user-visible one.

## Limits and flow control

| Limit | Value |
|-------|-------|
| Submission chunks | 1–4096 |
| Maximum chunk size | 4 MiB |
| Maximum IB length | `0x00ff_ffff` dwords |
| Fence buffer | Exactly one page |
| Maximum resident buffers | Kernel limit; a driver MUST split its own work if the list is rejected. |

A submission that exceeds a limit is rejected as a whole; the kernel does not
split it.

## Ring reuse

A ring slot may be reused after the submission that occupied it has signalled
its fence. A pipeline that reuses a slot earlier can overwrite a command word
that the command processor has not consumed.

> UNRESOLVED: the exact read-cursor update granularity of each pipe, and whether
> a partially consumed packet can be seen by the producer.
