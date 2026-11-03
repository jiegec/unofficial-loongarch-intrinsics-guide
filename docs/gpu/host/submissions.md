# Submissions

A submission is one `CS` ioctl: a context, an optional residency set, and an
array of chunks. The chunks name indirect buffers, the buffer that receives the
completion value, and the fences that order the work. This chapter covers the
array, admission, what a buffer costs in the ring, what order is promised, and
what a sequence number proves. Structures are in
[User-space interface](uapi.md#submission); pipe words are in [GPIPE](gpipe.md).

## Chunk array

`num_chunks` MUST be 1 through 4096. `chunks` is an array of 64-bit user
addresses; each points at a 16-byte `drm_loonggpu_cs_chunk` — `chunk_id`,
`length_dw` in 32-bit words, `chunk_data`. The array is copied first, then each
header and payload; length and id are checked before the payload is copied.

| ID | Chunk | Count | Payload |
|----|-------|-------|---------|
| `0x01` | `IB` | one record, MAY repeat | `drm_loonggpu_cs_chunk_ib` |
| `0x02` | `FENCE` | one record | `drm_loonggpu_cs_chunk_fence` |
| `0x03` | `DEPENDENCIES` | one or more | `drm_loonggpu_cs_chunk_dep` |
| `0x04` | `SYNCOBJ_IN` | one or more | `drm_loonggpu_cs_chunk_sem` |
| `0x05` | `SYNCOBJ_OUT` | one or more | `drm_loonggpu_cs_chunk_sem` |
| `0x06` | `BO_HANDLES` | header plus entries | `drm_loonggpu_bo_list_in` |
| `0x08` | `SYNCOBJ_TIMELINE_WAIT` | one or more | `drm_loonggpu_cs_chunk_syncobj` |
| `0x09` | `SYNCOBJ_TIMELINE_SIGNAL` | one or more | `drm_loonggpu_cs_chunk_syncobj` |

Every chunk except `IB` is a singleton. A fixed-size chunk carries exactly one
record, an array chunk a whole number of records; `0x07` and every unknown id
are refused.

A `DEPENDENCIES` or `SYNCOBJ_IN` chunk adds waits; a `SYNCOBJ_OUT` or
`SYNCOBJ_TIMELINE_SIGNAL` chunk attaches this submission's fence to a sync
object; a `FENCE` chunk names the completion buffer. The residency set comes
from `bo_list_handle` or from a `BO_HANDLES` chunk, never both; with neither
it holds only the page directory and the fence buffer, so no client buffer can be
admitted.

## Engine selection

`ip_type`, `ip_instance` and `ring` select the ring. `LOONGGPU_HW_IP_GFX` (0),
`LOONGGPU_HW_IP_DMA` (2) and `LOONGGPU_HW_IP_BPIPE` (8) name the graphics,
transfer and blit rings; `LOONGGPU_HW_IP_COMPUTE` (1) is refused with `-ENODEV`
before the other fields are read, because compute is reached through `USERQ`
([Compute submission](compute-submission.md)). `LOONGGPU_HW_IP_DPIPE` (5) and
`LOONGGPU_HW_IP_EPIPE` (6) are defined with no engine on this generation and
answer `-ENODEV`; any other value is `-EINVAL`.

`ip_instance` and `ring` MUST be 0, every engine of this generation having one
ring. All IB chunks MUST name the same ring — a second ring is refused with
`-EINVAL` — and a submission with no IB chunk is refused with `-EINVAL`. An engine that is
reloading or parked for recovery refuses new work with `-EAGAIN`, distinct from
`-ENODEV`.

## Admission

Admission runs after the residency set is built and before anything is emitted; a
failure refuses the whole submission.

| Check | Refusal |
|-------|---------|
| `ib_bytes` nonzero and a multiple of 4 | `-EINVAL` |
| `ib_bytes` at most `0x00ff_ffff` dwords | `-E2BIG` |
| `va_start` 4-byte aligned, no overflow | `-EINVAL` |
| one present mapping covers the range | `-ENOENT`, `-EACCES`, `-ERANGE` |
| the backing buffer is in the residency set | `-EACCES` |
| the range lies inside that buffer | `-ERANGE` |
| only implemented flag bits | `-EINVAL` |

Presence is the whole permission: there is no execute bit and the writeable bit
is not inspected. The buffer is executed where it lies, not staged through a
copy, and its contents are not parsed; it MUST stay mapped until the submission
retires.

Only `CE` (`1 << 0`) and `PREAMBLE` (`1 << 1`) pass admission; `PREEMPT`
(`1 << 2`) and `TC_WB_NOT_INVALIDATE` (`1 << 3`) are refused with `-EINVAL`.
Neither accepted bit does what its name suggests: the engine comes from
`ip_type`, and no ring of this generation emits a context-control packet, so a
preamble buffer runs like any other. A client MUST NOT rely on either bit.

## Ring packet

A launch is five words on graphics and blit — a VM select and a four-word buffer
packet — and four on transfer, which has no select. The buffer packet carries the
address and a control word whose `[23:0]` is the dword count and `[31:24]` the
VMID; that split is why the length bound is `0x00ff_ffff`
([GPIPE](gpipe.md#indirect-buffer)).

The frame also carries a page-directory flush ahead of the buffers when needed,
and the writeback packets behind them. The ring reserves room for the whole frame
and publishes it as one unit, so a frame that does not fit is dropped, not
truncated.

## Ordering

Buffers run in chunk order; a frame runs the flush, the buffers, the fence-buffer
writeback, then its own fence.

A ring is one command stream: frames are published one at a time and consumed in
that order, so packets from two submissions never interleave. That is stream
order, not work order — the pipe may still execute what a frame launched after its
last packet was fetched, so a fence proves consumption, not a drained pipe.

Each context has one scheduler entity per ring. Up to two frames may be
outstanding per ring, a host policy; past that the ring refuses to build the frame
and the submission fails through its fence. Which queued submission is built next
is the scheduler's choice.

Across rings, order comes only from a `DEPENDENCIES` record, a sync-object wait,
or the fences already pending on a shared buffer. An order a client needs MUST be
one of those.

## Completion

`out.handle` is a 64-bit sequence, allocated per context and ring from 1 upwards;
it names the submission in `WAIT_CS`, `WAIT_FENCES`, `FENCE_STATUS`,
`FENCE_TO_HANDLE` and in a dependency record.

A `FENCE` chunk names a buffer and an offset. The buffer MUST be exactly one page
and MUST NOT come from a user pointer; the offset MUST leave room for 64 bits. The
engine writes the sequence there after the last buffer and before the submission's
own fence. The store is 32 bits wide and the upper four bytes are untouched, so a
client SHOULD compare the low 32 bits of the handle.

That word is a progress marker, not a completion record. A ring timeout completes
the outstanding fences with errors and leaves the word untouched, and the word is
written before the submission's fence, so it can arrive first. A buffer MUST NOT
be reused because the word arrived; only retirement says so
([User-space interface](uapi.md#waiting-and-completion)).

A submission is admitted in the device's current epoch. If the epoch changes
before the ring runs it, it is not emitted and its fence fails with `-EAGAIN`; if
local memory was lost since the context was created, it fails with `-ECANCELED`.
An older epoch's completion is no proof about the current device state.

A wait takes an absolute `CLOCK_MONOTONIC` deadline in nanoseconds, `~0ull`
meaning none; a deadline already past returns status 0 and cancels nothing. A
finished error is returned in place of the status.

## Limits

| Limit | Value |
|-------|-------|
| Chunks per submission | 1–4096 |
| Payload per chunk | 4 MiB (`1 << 20` words) |
| Indirect buffer | 1 to `0x00ff_ffff` dwords |
| Buffers per submission | the engine's `LOONGGPU_INFO_MAX_IBS` value |
| Fence buffer | one page; offset at most one page less 8 bytes |
| Rings per submission | one |

The per-engine buffer count is a framing budget, not an admission rule: a frame
larger than the ring's budget cannot be published, so exceeding it costs the
submission after it was accepted.

## Failure

| Condition | Result |
|-----------|--------|
| Accelerator not working | `-EBUSY` |
| Fault-storm window, or compute pipe quarantined | `-EAGAIN` |
| Engine not accepting work | `-EAGAIN`, retryable |
| `COMPUTE`, or no registered engine | `-ENODEV` |
| Malformed or repeated chunk, mixed rings, no IB, unknown context | `-EINVAL` |
| Context guilty, or local memory lost | `-ECANCELED` |
| Buffer not in the residency set, or the mapping is not present | `-EACCES` |
| No mapping at the start, or the range leaves its mapping or buffer | `-ENOENT`, `-ERANGE` |
| Chunk copy, allocation | `-EFAULT`, `-ENOMEM` |
| Frame does not fit, or no frame slot | fence error, after the handle |

A refusal is atomic: the ioctl returns before a sequence is issued and nothing is
emitted. Once a handle exists, later failures — an untranslatable buffer, a frame
the ring cannot hold, an engine that stopped accepting — arrive through the fence.

## Open questions

- UNRESOLVED: whether a buffer marked preamble is ever skipped; no packet carries
  the mark on this generation, so a client cannot tell.
- UNRESOLVED: how much of a buffer ran when a submission is cancelled while
  executing.
- UNRESOLVED: whether the fence word is written at issue or at drain
  ([Rings](rings.md#completion)).