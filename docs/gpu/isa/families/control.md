# Control and Ordering Family

This family changes control flow, orders pending work and reads execution
environment state. The rows are in the
[control index](../../instructions/control-prefix.md).

## Branches

`b` carries a signed displacement in `INST_LO[31:0]`, measured in eight-byte
instructions from the instruction that follows:

```text
target = branch_address + 8 + displacement * 8
```

There is no delay slot. `cb` branches when its condition is nonzero; `czb`
branches when it is zero. The condition is encoded in `INST_HI[7:0]` and may be
a lane mask or `SCC`. A branch on a lane mask acts on the mask as a value.

## Ordering

`check` drains pending work named by four domain bits:

| Bit | Domain |
|----:|--------|
| 0 | Kernel and scalar. |
| 1 | Vector memory. |
| 2 | Vector store. |
| 3 | Shared memory. |

The bits are independent and may be combined. `check` is not a host fence, does
not signal completion and is not a lane rendezvous. It orders the current
invocation's work.

## Termination

`exit` ends the invocation. A stage's normal path ends with an export, a
`check` and an `exit`, in that order.

A discard is not a termination: the shader restores the captured mask, clears
the lanes that must not write and continues. `exit` is not executed on that
path.

## Barriers

`barrier` synchronises the invocations of a workgroup. It is the ordering
mechanism for shared memory: a store before the barrier is visible to other
invocations after it. It is not a global memory fence.

## Jump stack

`jpush` and `jpop` push and pop a control-flow record. The record value is in
the low word and a predicate in the high word. The resume consumes saved operand
state, not a displacement.

> UNRESOLVED: what the jump-stack predicate means and whether it gates the
> push or pop itself.

## Traps

`trap` raises the encoded shader trap; `rfe` is the return edge. Trap delivery
requires the debug and trap interface to be enabled and carries process and
queue attribution. A trap is not a completion event and does not signal a fence.

## Environment reads

| Instruction | Reads |
|-------------|-------|
| `getpc` | The current control-flow position into a scalar-register pair. |
| `getreg` | The environment register selected by an encoded immediate into a scalar destination. |
| `s_movindex_b32` | A dword of an environment-supplied window, selected by an index. |

The compute identifiers are not reached through `getreg`; they come from the
kernarg block, the group-id SGPRs and the local-id VGPRs. See
[Compute](../../shaders/compute.md).

> UNRESOLVED: the `getreg` hardware-register numbering and the meaning of the
> environment window for a graphics stage.

## Prefix instructions

`prefix_sw`, `prefix_tp4` and `prefix_tp8` carry a 32-bit literal in the low word
and select their operation from two high-word bits. The literal is consumed by
the operation that the prefix defines.

> UNRESOLVED: what each prefix operation does to its literal.

## Shared-memory copies

`copy_sm_from_global` and `copy_sm_to_global` copy a block between workgroup
shared memory and global memory. Their operand form is a shared-memory side and
a global side.

> UNRESOLVED: the transfer granularity, the global-side address form and the
> ordering requirements.

## Family map

| Instruction | Purpose |
|-------------|---------|
| `b`, `cb`, `czb` | Branches. |
| `check` | Order pending work. |
| `exit` | End the invocation. |
| `barrier` | Workgroup rendezvous. |
| `jpush`, `jpop` | Save and restore a control-flow record. |
| `trap`, `rfe` | Raise and return from a trap. |
| `getpc`, `getreg` | Read launch state. |
| `flag` | Update execution-domain flag state. |
| `nop` | Hold an instruction slot. |
| `prefix_sw`, `prefix_tp4`, `prefix_tp8` | Carry a literal for the following operation. |
| `copy_sm_from_global`, `copy_sm_to_global` | Move blocks between shared and global memory. |

## Open questions

- The jump-stack predicate.
- The prefix operations' effects.
- The shared-memory copies' granularity and address form.
- The `flag` and `rfe` forms.
- Whether the barrier has any effect outside a compute workgroup.
