# Control Flow and Ordering Family

The instruction index for this family is under [control-prefix](../instructions/control-prefix.md).

This family changes control flow, orders pending work, and reads execution
environment state.  The conditional-execution mechanism it builds on is defined
in [Operands and Execution Registers](operands.md#conditional-execution).

## Branches

`b` carries a signed displacement in the low word, measured in instructions of
eight bytes and counted from the instruction that follows the branch:

```text
target = branch_address + 8 + displacement * 8
```

`b` has no other operand.  `cb` and `czb` add a condition register, encoded in
`INST_HI[7:0]`; `cb` branches when it is nonzero and `czb` when it is zero.  The
register is a lane-mask register or the scalar condition register, exactly as in
any other general source position.

Because the condition lives in a register rather than in a flags word, `cb` and
`czb` are normally preceded by a compare, and the compare's result must be
materialised before the next compare is issued.  A branch whose condition is a
lane mask acts on the mask's value, not on a per-lane predicate.

## Ordering

`check` is the ordering instruction.  It drains the pending kernel, VM,
vector-store and shared-memory work named by its encoded domain bits before the
program continues.  The four bits are `X0_1` (bit 0, `k`, the kernel/scalar
class), `M.vm1` (bit 1, `vm`, the vector-memory class), `X1_2[0]` (bit 2,
`vs`, the vector-store class) and `X1_2[1]` (bit 3, `sm`, the
shared-memory class).  It is the mechanism behind three rules that appear
throughout this documentation:

* After a memory or sampling operation, before its result is used.
* After a descriptor fetch, before the descriptor is read.
* After an interface write such as an export or an emit, before the next write
  reuses the same registers.

`check` is not a fence in the host sense and does not signal anything: it orders
work within the invocation.  It is also not a lane rendezvous; that is
`barrier`.  The vendor reference producer names the classes in its own
assembly (`llc -march=lggpu`: `check k|vm|vs|sm(15)`, `check k(1)`,
`check vm(2)`, `check vs(4)`), and its cached kernels carry the operand
values 0x1, 0x2, 0x3, 0x4 and 0x8: each class is selected independently or in
combination.  What a zero operand orders (nothing, or a conservative
all-class wait) is UNRESOLVED; in the cached kernels it appears only where
none of the listed work was pending.

## Termination

`exit` ends the invocation.  A stage's normal path ends with an export, a
`check` and an `exit`, in that order.

Discarding fragments is not a termination: the shader restores the mask it
captured at entry, clears the lanes that must not write, and continues.  `exit`
is not executed on that path.

## Barriers

`barrier` synchronises the invocations of a workgroup.  It is the ordering
mechanism for workgroup shared memory: a store performed before the barrier is
visible to other invocations after it.  The barrier is a workgroup operation,
not a memory fence for global memory.

## Jump stack

`jpush` and `jpop` push and pop a return address, with a displacement carried in
the low word and a predicate in the high word.  The displacement is an unsigned
field in the machine encoding, and the operand layout also reserves a scalar
position and a row-defined byte.  The exact meaning of the predicate and the
interaction with the scalar operand are UNRESOLVED.

## Traps

`trap` raises the encoded shader trap, and `rfe` is the return edge of the path
it enters.  Trap delivery requires the debug and trap interface to be enabled
and carries process and queue attribution; a trap is not a completion event and
does not signal a fence.

## Environment reads

Three instructions read execution-environment state, only two of which are in
this family:

| Instruction | Reads |
|-------------|-------|
| `getpc` | the current control-flow position into a scalar-register pair |
| `getreg` | the environment register selected by an encoded immediate into a scalar destination |
| `s_movindex_b32` | a dword of an environment-supplied window, selected by an index; this row belongs to the [scalar ALU family](family-scalar.md) |

These instructions read launch and environment state rather than ordinary
registers; the namespace of the environment registers, and which of them are
meaningful during a graphics stage, is UNRESOLVED.

## Prefix instructions

`prefix_sw`, `prefix_tp4` and `prefix_tp8` carry a 32-bit literal in the low word
and select their operation from two high-word bits.  The literal is consumed by
the operation that the prefix defines; it does not turn a host packet or a
second instruction word into a shader instruction.  The defined prefixes and
their effects are UNRESOLVED beyond their names.

## Shared-memory copies

`copy_sm_from_global` and `copy_sm_to_global` copy a block between workgroup
shared memory and global memory, in the two directions.  Their operand form is a
shared-memory side and a global side; the transfer granularity, the descriptor or
address form of the global side, and the ordering requirements are UNRESOLVED.

## Family map

| Instruction | Purpose |
|-------------|---------|
| `b`, `cb`, `czb` | unconditional and conditional branches |
| `check` | order pending memory and interface work |
| `exit` | end the invocation |
| `barrier` | synchronise a workgroup |
| `jpush`, `jpop` | save and restore a return address |
| `trap`, `rfe` | raise a shader trap and return from it |
| `getpc`, `getreg` | read launch state |
| `flag` | update execution-domain flag state |
| `nop` | hold an instruction slot |
| `prefix_sw`, `prefix_tp4`, `prefix_tp8` | carry a literal for the following operation |
| `copy_sm_from_global`, `copy_sm_to_global` | move blocks between shared and global memory |

## UNRESOLVED

* The jump-stack predicate and its scalar operand.
* The environment register namespace of the environment read and index forms.
* The defined prefix operations and the effect of their literals.
* The transfer granularity and address form of the shared-memory copies.
* The `flag` and `rfe` forms.
* Whether `check` may be placed inside divergent code, and what a zero
  domain operand orders.
* Whether the barrier has any effect outside a compute workgroup.
