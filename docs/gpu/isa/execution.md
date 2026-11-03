# Execution

This chapter defines how an instruction selects the lanes it acts on, how a
branch works, and how the condition register and the ordering instructions
behave. The mask field's position differs by machine format; the semantics are
the same.

## Mask selector

An instruction carries a 2-bit selector that names one of `p0..p3`:

| Format | Field position |
|--------|----------------|
| `vop` | `INST_HI[13:12]` |
| `vop-imm`, `smem`, `flat`, `tex-buffer`, `shared-memory` | `INST_HI[17:16]` |
| `sio-id`, `sio-imm` | `INST_HI[21:20]` |
| `jump-stack` | `INST_HI[17:16]` |

The selected mask is an **absolute** lane set. It is not intersected with `p0`,
and it is not a per-lane predicate evaluated by the instruction.

## Effects of a mask

- The instruction executes only on the lanes whose bit is set.
- A write on a lane outside the mask is suppressed; the destination keeps its
  previous value. The instruction does not clear the lane.
- The selected mask register is not modified by the instruction.
- Two gated writes with complementary masks compose into a per-lane select.

Because a gated write suppresses rather than clears, a merge of two regions is
one gated copy per predecessor mask, and a helper-lane region widens the live
mask and restores it without cleaning up the values.

## Mask registers

- `p0..p3` are lane-mask registers and may gate any instruction.
- `p0` holds the lanes still alive for the invocation by convention; the other
  three hold region masks.
- The mask registers live in the scalar file but use the distinguished selector
  encodings `0x7c..0x7f`; they are not a window of `s0..s3`.
- `p1..p3` are undefined at launch. A shader MUST write a mask before selecting
  it.

A compare can write a mask register. If the instruction is gated by that same
register, the selector is read before the write, so the result is
`old_mask & condition`.

A whole-quad-mask instruction widens a mask to the quads that contain its set
lanes; its inverse narrows a mask. Both take and return mask registers.

## Condition register

`SCC` is one bit per wave. Compare instructions write it; add-with-carry,
subtract-with-borrow, select and conditional branches read it.

- The value is a flag, not a bit of the arithmetic result. A carry or borrow is
  "the condition is non-zero".
- The register is short-lived across compares: a consumer MUST be materialised
  before the next compare.
- A compare that writes a scalar register rather than `SCC` is not the
  established form.

## Branches

A branch carries a signed 32-bit displacement in `INST_LO[31:0]`. The unit is one
instruction (eight bytes), measured from the instruction that follows the branch:

```text
target = branch_address + 8 + displacement * 8
```

There is no delay slot. The instruction after a branch is not executed as part
of it.

`b` is unconditional. `cb` branches when its condition is nonzero; `czb` branches
when it is zero. The condition is encoded in `INST_HI[7:0]` and may be a lane
mask or `SCC`. A branch on a lane mask acts on the mask as a value; it is not a
per-lane branch.

## Divergence

A divergent region is a narrowed `p0`. It is entered by a compare that writes a
region mask and a branch on that mask, and left by restoring the saved mask.
Instructions inside the region select the region mask.

An ordering instruction may be placed inside a narrowed region. It orders that
region's work; an ordering instruction outside the region does not order work
inside it.

## Ordering and barriers

The ordering instruction drains the pending work named by its four domain bits:

| Bit | Domain |
|----:|--------|
| 0 | Kernel and scalar. |
| 1 | Vector memory. |
| 2 | Vector store. |
| 3 | Shared memory. |

The bits are independent and may be combined. The instruction does not signal
completion and is not a host fence.

A workgroup barrier synchronises the invocations of one workgroup. It is the
ordering mechanism for shared memory. It is not a global memory fence and not a
lane rendezvous for an arbitrary mask.

## Open questions

- Whether the fragment pipe's helper-lane widening interacts with a region mask
  in every family.
- Whether an SIO transfer is gated by the mask selector like an ALU operation.
- The exact behaviour of a compare that writes the same mask it selects, for
  every format.
