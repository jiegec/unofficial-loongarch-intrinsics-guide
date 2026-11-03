# Submissions

A submission is a `CS` ioctl that carries a set of chunks. This chapter defines
the submission's composition, ordering and failure rules. The chunk structures
are in [User-space interface](uapi.md#submission); the ring that receives the
work is in [Rings](rings.md).

## Composition

A submission has a context, a residency list and 1 through 4096 chunks. It MUST
target one ring: every IB chunk carries the same `ip_type`, `ip_instance` and
`ring`. A submission that mixes rings is rejected.

| Chunk | Role |
|-------|------|
| `IB` | The indirect buffer to execute; one or more, in order. |
| `FENCE` | The buffer that receives the completion sequence number. |
| `DEPENDENCIES` | Fences that must signal before this submission starts. |
| `SYNCOBJ_IN` | Binary sync objects to wait on. |
| `SYNCOBJ_OUT` | Binary sync objects to signal. |
| `SYNCOBJ_TIMELINE_WAIT` | Timeline point to wait on. |
| `SYNCOBJ_TIMELINE_SIGNAL` | Timeline point to signal. |
| `BO_HANDLES` | Buffer handles used by the submission. |

The IB chunk's `flags` select the copy engine, preamble, preemption and
writeback behaviour. The IB's GPU address MUST be mapped executable and its
length MUST be a multiple of four bytes.

## Ordering

The kernel admits the submission only when every buffer in the residency list is
resident and every dependency is satisfied. The IB chunks execute in the order
they appear. A fence chunk is written after the work; a sync-object signal is
appended as the last signal in the chain.

A submission is serialised against the previous submission on the same ring
unless the host explicitly overrides the dependency. Two submissions on
different rings are independent unless a dependency or a shared sync object
orders them.

## Limits

| Limit | Value |
|-------|-------|
| Chunks per submission | 1–4096 |
| Bytes per chunk | At most 4 MiB |
| IB length | At most `0x00ff_ffff` dwords |
| Fence buffer | Exactly one page |
| Ring count | One ring per submission |
| Residency | The kernel rejects a list that does not fit; it does not split the submission. |

## Completion

The `CS` ioctl returns a 64-bit sequence number. The fence chunk's buffer
receives the number when the submission completes. A waiter may:

- poll the fence word;
- wait on the sync object with an absolute deadline;
- wait on the fence through `WAIT_CS` or `WAIT_FENCES`.

A zero timeout means "poll once". A timeout is not a retry: the context is
marked lost, the kernel is asked whether a reset occurred, and the context is
rebuilt only after a reset is reported.

A submission's completion belongs to the epoch in which it was admitted. A
completion from an older epoch MUST NOT be treated as proof that the current
device state executed the work.

## Failure

| Condition | Result |
|-----------|--------|
| Invalid chunk or mixed rings | Rejected before admission. |
| Buffer not resident | Rejected or made resident before admission, according to the list. |
| IB not executable | Rejected. |
| Queue full or scheduler busy | `-EAGAIN`; the submission may be retried. |
| Context or device lost | `-ECANCELED`; the submission will not complete. |
| GPU fault | The context is quarantined and a reset may follow. |

## Open questions

- The exact ordering between a timeline signal and the fence writeback.
- Whether a submission may contain more than one fence chunk.
- The behaviour of a submission that is cancelled while its IB is partially
  executed.
