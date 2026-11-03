# Control and Ordering Family

This family changes control flow, orders issued work, synchronises a workgroup,
ends an invocation and carries two launch-contract block transfers. Word layout
is in [Encoding](../encoding.md), the selector space in [Operands](../operands.md)
and the execution model in [Execution](../execution.md).

## Rows

| Row | Operand fields | Effect |
|-----|----------------|--------|
| `b` | `IO32` LO[31:0] | Unconditional relative branch. |
| `cb` | `GJ8` HI[7:0], `IO32` LO[31:0] | Branch when the condition is non-zero. |
| `czb` | `GJ8` HI[7:0], `IO32` LO[31:0] | Branch when the condition is zero. |
| `jirl` | `RJ8x2` HI[7:0], `RD8x2` HI[15:8], `IO32` LO[31:0] | Return position plus branch to a pair. |
| `getpc` | `RD8x2` HI[15:8] | Control-flow position to a scalar pair. |
| `getreg` | `IJ32` LO[31:0], `RD8` HI[15:8] | Environment register named by the literal. |
| `jpush` | `RA8` LO[7:0], `X2_23` LO[30:8], `X0_1` LO[31], `X1_8` HI[15:8], `PJ2` HI[17:16] | Push a control-flow record. |
| `jpop` | as `jpush` | Pop a record and resume. |
| `check` | `X0_1` LO[0], `M.vm1` LO[1], `X1_2` LO[3:2] | Order pending work in the named domains. |
| `flag` | as `check` | Update execution-domain state. |
| `barrier` | none | Workgroup rendezvous; orders its shared memory. |
| `exit` | none | End the invocation. |
| `trap` | `RJ8x2` HI[7:0], `RD8x2` HI[15:8], `IO32` LO[31:0] | Enter the trap path. |
| `rfe` | `GJ8x2` HI[7:0], `IO32` LO[31:0] | Return from the trap path. |
| `nop` | none | Hold an instruction slot. |
| `prefix_sw`, `prefix_tp4`, `prefix_tp8` | `IJ32` LO[31:0] | Install SW, TP4 or TP8 prefix state. |
| `copy_sm_from_global` | `PJ2` HI[17:16] | Launch region into workgroup memory. |
| `copy_sm_to_global` | `PJ2` HI[17:16] | Workgroup memory back to the launch region. |

Thirteen rows are `sop-imm-control`; `jpush` and `jpop` are the two `jump-stack`
rows, the copies are `shared-memory` and the prefixes are `prefix`. A row with no
operand is its base word: `barrier` is `INST_HI` 0x402f0000, `exit` 0x402e0000 and
`nop` 0x40300000. `flag` takes the four bits of `check` but updates
execution-domain state, including the memory selector, instead of ordering, and
only its all-zero form has an established encoding. `nop` holds a slot and orders
nothing, so it is not a substitute for `check`.

## Branches

`b`, `cb` and `czb` share one arithmetic. The displacement is a signed 32-bit
count of eight-byte instructions in `INST_LO[31:0]`, measured from the
instruction after the branch:

```text
target = address(branch) + 8 + 8 * displacement
```

Zero continues at the next instruction. The field spans -2^31 through 2^31-1
instructions, so a farther target needs an intermediate branch, and a producer
MUST range-check the value before truncating it. There is no delay slot: a taken
edge does not execute the slot that follows it, so the slot after an
unconditional branch is reachable only as a branch target, while the slot after
a conditional branch is its fall-through.

`cb` takes the edge when its condition is non-zero and `czb` when it is zero.
The condition is an eight-bit general-source selector in `INST_HI[7:0]` — an
SGPR, an inline constant, a mask register `p0`..`p3` or `SCC`. It is one 32-bit
word for the whole wave, so no encoding makes the edge per lane: a mask condition
asks whether any lane is still active, because an empty mask reads as zero, and a
producer that wants one bit must mask the value first. Divergence is therefore a
narrowed mask plus an unconditional branch.

Whether a condition written by the immediately preceding instruction is visible
to the branch is UNRESOLVED, and no ordering operation is established to close
that gap, so a producer MUST NOT depend on the pair being adjacent.

## Ordering

The ordering row is `check`. It carries its domain in four bits and has no other
operand.

| Bit | Field | Domain |
|----:|-------|--------|
| 0 | `X0_1` LO[0] | Scalar registers and kernel memory. |
| 1 | `M.vm1` LO[1] | Vector memory. |
| 2 | `X1_2[0]` LO[2] | Vector store. |
| 3 | `X1_2[1]` LO[3] | Shared memory. |

The bits combine; 0xf names all four and is the value a producer SHOULD use when
the pending class is unknown. Each set bit drains and orders that class of work
already issued, so a following consumer sees it. A zero domain has no established
meaning: whether it orders nothing or waits for everything is UNRESOLVED.

`check` is not a lane rendezvous and not a host fence. It writes no register and
produces no value. It orders work issued on the executing path, so a wait placed
before a region cannot order work issued inside that region. The row has no
`PJ2`, so a wait
cannot be gated: the mask decides which lanes reach it, not what it orders. It
does not make one invocation's shared-memory writes visible to another — that is
the barrier's job.

## Barriers

`barrier` has no operand, no mask selector and no scope field. It is one fixed
word. A wave that reaches it waits until the other invocations of its workgroup
arrive, and workgroup memory written before it is visible to accesses issued after
it, so it separates one invocation's store to the shared region from another's
load. The scope is fixed at the workgroup: a barrier orders nothing outside it, so a
memory scope that reaches further also needs the full-domain `check`, and whether
the machine requires a barrier word ahead of a workgroup barrier is UNRESOLVED.

The row takes no mask, so the barrier cannot be predicated: it issues for the wave
or the wave does not reach it. The hazard is the workgroup. Two invocations that
reach different barriers, or a different number of them, cannot both complete,
and the instruction encodes no participant count, no timeout and no partial
arrival. Whether an invocation that already executed `exit` counts as an arrival
is UNRESOLVED, and so is whether a workgroup completes when the launch masked off
part of it. A barrier on a path only some invocations take therefore has no
established completion, so a shader MUST keep it in convergent control flow for
the whole workgroup.

## Termination

`exit` has no operand, no mask and no condition, so the encoding cannot make it
partial. It is a block terminator: nothing of its block follows it, and every path
MUST end in a terminator. The last word has no successor, so a path that reaches
the end of the body keeps fetching instead of ending; what it executes next is
UNRESOLVED and a producer MUST NOT rely on it.

`exit` encodes no wait. Whether it waits for outstanding loads, stores, atomics
and stream traffic, or leaves them to the stage interface, is UNRESOLVED. A shader
whose result depends on pending memory MUST place the ordering operation before
it; a full-domain `check` covers all four classes. Whether `exit` retires the
lanes still marked active or the invocation as a whole is UNRESOLVED, as is a
second `exit` on an invocation that already terminated.

## The jump stack, `jirl` and `getpc`

Both `jump-stack` rows carry the record in the scalar register named by `RA8`,
the encoded position in `X2_23` and `PJ2`; neither has a destination.
`jpush` saves a control-flow position with the operand state the resume consumes
and `jpop` removes the record and resumes the described path, so the two MUST
nest. `jirl` calls through a register: it writes the position of the instruction
after itself to the pair in `RD8x2` and continues at the pair in `RJ8x2` plus the
32-bit field, so a callee that clobbers the return pair cannot return. `getpc`
writes a control-flow position to the pair at `INST_HI[15:8]`.

UNRESOLVED: whether `X2_23` is a displacement in the branch unit or saved operand
state (the readings agree at zero, and every seed passes zero); the roles
of `X0_1` and `X1_8`; whether `PJ2` gates the push and pop or selects a stack
slot; whether `jirl` is tracked by the jump stack; the unit of its field and
whether the pair is a base or the whole target; stack depth, a pop with no push
and an unbalanced stack at termination; and, for `getpc`, whether the value is
its own position or the next instruction's. A position is not a data address, so
turning a position into an address needs a translation the rows do not perform.

## Traps

`trap` leaves the ordinary stream for the trap path and `rfe` is the return edge.
Both carry a 32-bit displacement in `INST_LO[31:0]`; `rfe` follows the branch rule
of [Encoding](../encoding.md), while the unit of `trap`'s field is UNRESOLVED.
`trap` names a pair at `INST_HI[7:0]` and a second at `INST_HI[15:8]`; which is
read and which receives a result is UNRESOLVED. `rfe` reads the pair at
`INST_HI[7:0]` as the state to restore.

UNRESOLVED: what that pair must contain, what state is restored, whether the trap
path runs in the same invocation state and returns at all, whether delivery is per
lane or per wave, `rfe` on a diverged path, and an `rfe` with no preceding
`trap`. A shader MUST NOT use a trap as a subroutine call.

## Environment reads

`getreg` reads one 32-bit environment register into the scalar destination at
`INST_HI[15:8]`, selected by the literal at `INST_LO[31:0]`. The selector is an
encoded constant, so a computed selector needs a branch over a sequence of rows,
and no paired or wider form exists. Which selector supplies which value, and
whether a graphics stage sees the same namespace, is UNRESOLVED.

The identifiers a compute shader needs do not come through `getreg`: the group id
is in `s11`..`s13`, the local invocation id in `v0`..`v2`, and the grid and group
sizes are fields of the implicit-argument block reached from the kernarg pointer in
`s[6:7]`. See [Compute](../../shaders/compute.md).

## Prefix rows

The three prefixes are one format with a two-bit opcode field at `INST_HI[15:14]`
and one 32-bit literal at `INST_LO[31:0]`; they differ only in the opcode. A
prefix changes how the following word is read and writes no register, memory or
control-flow state.

The one established pair is a TP4 prefix followed by a move, and the prefix's
literal differs only in the low byte across the three lanes of a quad: 0x0001fe00,
0x0001fe55 and 0x0001feaa. That byte is a two-bit lane index repeated four times
(0, 1 and 2), and bits 16:9 are set in all three, so the established use selects a
source lane. What the remaining bits select, and what TP4 and TP8 change relative
to SW, is UNRESOLVED. So is whether one operation consumes the state, whether it persists
until the next prefix, and whether two prefixes compose. A producer MUST emit
exactly one prefix immediately before the operation it affects and MUST NOT
separate the pair with another word, place a prefix at a branch target, or leave
one at the end of a body.

## Shared-memory copies

`copy_sm_from_global` and `copy_sm_to_global` name no address, no extent and no
direction field: the launch contract supplies the region and the size, and the
only operand is the mask selector `PJ2` at `INST_HI[17:16]`. No register is
written, so the transfer has no completion test.

A lane MUST NOT read the region before the inbound copy has completed, and the
outbound copy MUST follow the last write to it and precede termination, because a
copy-out overlapping a lane's write leaves a partially updated region no ordering
rule covers. UNRESOLVED: when either copy completes, whether `PJ2` gates it or
selects a participant set, whether it runs per invocation or per workgroup, and
what the global side addresses are.

## Open questions

- The unit of `X2_23` and the roles of the other record fields.
- Whether `PJ2` gates the jump stack and the shared-memory copies.
- Whether `barrier` has any effect outside a compute workgroup.
- The `getreg` selector namespace and the prefix bits outside the lane index.
