# Compute

> **Scope.** This chapter is deliberately brief.  The compute path is the part of
> the command layer that is least established: the open driver's compute pipeline
> is not complete, so the grammar below is recorded from its command schema and
> from the probes that ran through it, and the semantics that are still open are
> marked rather than filled in.  It will be rewritten from code facts once the
> compute path is working end to end.

Compute work uses the CPIPE pipe.  Where graphics owns a ring that the kernel
drives directly, compute is queue-based: the kernel creates a queue, the queue's
user ring carries the compute command grammar, and the user driver publishes
`rptr`/`wptr` and rings a doorbell.  There is no user-visible BAR doorbell on this
generation, and the queue record is the kernel's business — see
[CPIPE](pipes/cpipe.md) for the queue and doorbell packets, and the
[generation profile](../chips/lg200.md#pipes) for the pipe's cursors and VMIDs.

## Dispatch

A dispatch is a pair of records on the compute queue's user ring.  The local-size
record declares the workgroup shape and the dispatch record declares how many
workgroups to launch:

| Record | Size | Contents |
|--------|-----:|----------|
| Local size | 8 bytes | a fixed command word, then three 10-bit local sizes (X, Y, Z) |
| Dispatch | 16 bytes | a fixed command word, then three 32-bit dimension fields |

The fixed command words are `0x00100189` for the local-size record and
`0x00000787` for the dispatch record, as emitted by the open driver.  Which axis
each of the three dispatch dimensions maps to is not independently established:
the schema records the roles as `UNRESOLVED` and notes that probes are needed to pin
them.  A driver must therefore not assume that the dimension fields are ordered
X, Y, Z.

## Completion

A compute queue completes through the pipe's event, which the kernel exposes as
the CPIPE interrupt source.  As for graphics, a submission's fence is what a user
driver waits on; the pipe event is what the kernel observes.

## What a compute shader sees

The shader side is the same stream of two-word instructions as any other stage,
starting at a kernel entry the launch supplies.  A compute invocation reads its
position from execution-environment state rather than from vector registers,
because there is no draw to supply `v0`/`v1`:

| Row | Supplies |
|-----|----------|
| `v_threadid_u32` | the lane's position within its wave |
| `s_movindex_b32` | a dword of the environment window selected by an index |

Which index selects which of the workgroup, local and global identifiers is
`UNRESOLVED`; see [Registers and Allocation](../shader/registers.md) and the
[scalar ALU family](../isa/family-scalar.md).

## Coverage

Compute is where the guide's most complete measurements were taken.  The
`load_sm_matrix_*` address model and the `v_mma_*` lane fragments were measured
through a compute submission, because the fragment-stage carrier cannot stage a
workgroup allocation; those results are recorded with the rows themselves in the
[vector ALU family](../isa/family-vector.md#matrix-multiply-accumulate) and
[memory and image family](../isa/family-memory.md#shared-memory).  The
[verification tooling](../appendix/verification.md) chapter describes the carrier
gap that remains.

## UNRESOLVED

* The mapping of the three dispatch dimension fields to axes.
* The local-size bounds: whether a 10-bit field is an architecture limit or a
  producer convention, and what an out-of-range value does.
* The compute queue record as a user-mode driver sees it, and the doorbell's
  acknowledgement.
* How a compute submission's completion relates to a graphics fence.
* Where each environment identifier actually comes from.  The identifiers are
  enumerated by the closed frontend (get_work_dim, get_global_size_*,
  get_local_size_*, get_local_id_*, get_num_groups_*, get_group_id_*,
  get_global_offset_*), and get_global_id_* is commented out there, so a global
  id must be built as group_id * local_size + local_id.  Compiling each builtin
  with the closed LLVM 18 lggpu backend shows where the value comes from - no
  hardware involved:

  | identifier | source |
  |---|---|
  | work_dim | kernarg block + 0x0, masked with 0x3 |
  | local_size_x / _y / _z | kernarg + 0x4 / + 0x6 / + 0x8, masked with 0xffff |
  | global_size_x / _y / _z | kernarg + 0xc / + 0x10 / + 0x14 |
  | global_offset_x / _y / _z | kernarg + 0x38 / + 0x3c / + 0x40 |
  | local_id_x / _y / _z | v0 / v1 / v2 - no load, the lane id is already there |
  | group_id_x / _y / _z | s4 / s5 / s6 - fixed scalar registers |
  | num_groups_* | NOTHING is emitted at all: the closed backend does not implement it |

  So the compute identifiers do NOT come from getreg indices: they come from a
  packed launch-state record addressed through the kernarg pair s[0:1], plus
  three fixed scalar registers and the three leading vector registers.
* Whether a compute stage uses the same shader-info record shape as a graphics
  stage, including its roots and arguments.
