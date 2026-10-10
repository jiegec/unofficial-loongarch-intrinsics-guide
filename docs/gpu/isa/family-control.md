# Control Flow and Ordering Family

The instruction index for this family is under [control-prefix](../control-prefix.md).

This family changes control flow, orders pending work, and reads execution
environment state.  The conditional-execution mechanism it builds on is defined
in [Operands and Execution Registers](operands.md#conditional-execution).

## Branches

A branch carries a signed displacement in the low word, measured in instructions
of eight bytes and counted from the instruction that follows the branch:

```text
target = branch_address + 8 + displacement * 8
```

An unconditional branch has no other operand.  A conditional branch adds a
condition register, encoded in `INST_HI[7:0]`; the register is a lane-mask
register or the scalar condition register, exactly as in any other general
source position.

Because the condition lives in a register rather than in a flags word, a
conditional branch is normally preceded by a compare, and the compare's result
must be materialised before the next compare is issued.  A branch whose
condition is a lane mask acts on the mask's value, not on a per-lane predicate.

## Ordering

The wait instruction has no operand in its established form and orders pending
memory and interface work before the program continues.  It is the mechanism
behind three rules that appear throughout this documentation:

* After a memory or sampling operation, before its result is used.
* After a descriptor fetch, before the descriptor is read.
* After an interface write such as an export or an emit, before the next write
  reuses the same registers.

The wait instruction is not a fence in the host sense and does not signal
anything: it orders work within the invocation.

## Termination

The terminate instruction ends the invocation.  A stage's normal path ends with
an export, a wait and a terminate, in that order.

Discarding fragments is not a termination: the shader restores the mask it
captured at entry, clears the lanes that must not write, and continues.  The
terminate instruction is not executed on that path.

## Barriers

A barrier synchronises the invocations of a workgroup.  It is the ordering
mechanism for workgroup shared memory: a store performed before the barrier is
visible to other invocations after it.  The barrier is a workgroup operation,
not a memory fence for global memory.

## Jump stack

The jump-stack pair pushes and pops a return address, with a displacement
carried in the low word and a predicate in the high word.  The displacement is an
unsigned field in the machine encoding, and the operand layout also reserves a
scalar position and a row-defined byte.  The exact meaning of the predicate and
the interaction with the scalar operand are UNRESOLVED.

## Traps

The trap instruction raises the encoded shader trap.  Trap delivery requires the
debug and trap interface to be enabled and carries process and queue
attribution; a trap is not a completion event and does not signal a fence.

## Environment reads

Two instructions read execution environment state:

| Instruction | Reads |
|-------------|-------|
| program counter read | the current program counter into a scalar register |
| environment register read | the environment register selected by an encoded immediate |

A third instruction selects a value through the environment's index state.
These instructions read launch and environment state rather than ordinary
registers; the namespace of the environment registers, and which of them are
meaningful during a graphics stage, is UNRESOLVED.

## Prefix instructions

The prefix family carries a 32-bit literal in the low word and selects its
operation from two high-word bits.  The literal is consumed by the operation
that the prefix defines; it does not turn a host packet or a second instruction
word into a shader instruction.  The defined prefixes and their effects are
UNRESOLVED beyond their names.

## Shared-memory copies

Two instructions copy a block between workgroup shared memory and global memory,
in the two directions.  Their operand form is a shared-memory side and a global
side; the transfer granularity, the descriptor or address form of the global
side, and the ordering requirements are UNRESOLVED.

## Family map

| Form | Purpose |
|------|---------|
| unconditional and conditional branches | control flow |
| wait | order pending memory and interface work |
| terminate | end the invocation |
| barrier | synchronise a workgroup |
| jump-stack push and pop | save and restore a return address |
| trap | raise a shader trap |
| program-counter and environment reads | read launch state |
| prefix forms | carry a literal for the following operation |
| shared-memory copies | move blocks between shared and global memory |
| flag and return-from-exception forms | UNRESOLVED |

## UNRESOLVED

* The jump-stack predicate and its scalar operand.
* The environment register namespace of the environment read and index forms.
* The defined prefix operations and the effect of their literals.
* The transfer granularity and address form of the shared-memory copies.
* The flag and return-from-exception forms.
* Whether the barrier has any effect outside a compute workgroup.
