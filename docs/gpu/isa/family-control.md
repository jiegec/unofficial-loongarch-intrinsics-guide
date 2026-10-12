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


**There is no delay slot.**  The target is measured from the instruction that
follows the branch, so the word after a branch is never executed as part of it.
The producer's output agrees where it can: in a whole linked program every `b` and
`jirl` is followed directly by a label.  A conditional `czb` is sometimes followed by
a run of `nop`s instead - but those are unreachable padding, and it is the target
formula above, not the census, that rules a delay slot out.
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
combination.  The class selection is now mapped to the fence scopes it comes from (2026-10-13,
compiling one kernel per scope): `mem_fence(CLK_GLOBAL_MEM_FENCE)` emits
`check k|vm|vs(7)` and `mem_fence(CLK_LOCAL_MEM_FENCE)` emits
`check k|vm|vs|sm(15)`; the `barrier(CLK_LOCAL_MEM_FENCE)` form emits `barrier` followed
by `check k|vm|vs|sm(15)`, and the global form the same with `(7)`.  So `sm` is selected
exactly when the local side is named, and `k|vm|vs` is the global side.  A scope naming
both emits both, in separate instructions (`check k(1)`, `check vm(2)`, `check vs(4)`).

**No zero operand appeared in any of those five programs**, which is what the cached
kernels already suggested.  So the open question is now narrower and specific: what
makes the producer emit a zero at all, and what the hardware then orders - which needs a
device.

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

`jpush` and `jpop` push and pop a control-flow record, with the record value carried in
the low word and a predicate in the high word.  Calling that value a displacement was
wrong and is corrected here: the rows say the resume consumes **saved operand state, not
a displacement**, and the field is unsigned in the machine encoding while the operand
layout also reserves a scalar position and a row-defined byte.

The record's encoding half is verified in both carriers - `jumpstack_check.py` runs clean
(problems 0): `PJ2` is the predicate, `RA8` the scalar operand, and the two opcodes
differ only in `0x40000`.  What the predicate MEANS, and how it interacts with that
scalar operand, needs a device.

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
registers.  **The identifier namespace is named, though not yet numbered** (closed frontend, `BuiltinsLGGPU.def`): the workitem builtins enumerate `get_work_dim`, `get_global_size_{x,y,z}`, `get_global_id_{x,y,z}`, `get_local_size_{x,y,z}`, `get_local_id_{x,y,z}`, `get_num_groups_{x,y,z}`, `get_group_id_{x,y,z}` and `get_global_offset_{x,y,z}`.  Notably the three `get_global_id_*` entries are COMMENTED OUT in that file, so a producer must build a global id as `group_id * local_size + local_id` rather than read one.  The `getreg` immediate packing is established (`size[4:0]`, `offset[9:5]`, `hw_reg[15:10]`), but the compute identifiers are not reached through the getreg immediate at all - see the Compute page for the record and the fixed registers they use: the tree holds no producer source or compiled kernel that calls the builtin, so the number can only be read from the closed lowering binary or from hardware.

## Prefix instructions

`prefix_sw`, `prefix_tp4` and `prefix_tp8` carry a 32-bit literal in the low word
and select their operation from two high-word bits.  The literal is consumed by
the operation that the prefix defines; it does not turn a host packet or a
second instruction word into a shader instruction.  The prefixes' EFFECTS are still UNRESOLVED, but their standing in the producer is not,
and the mode is now known to be fusion rather than standalone use.  The closed backend
exposes `prefix_tp4` and `prefix_tp8` as move builtins (`mov_i32`, `mov_u32`,
`mov_f32`) - and not `prefix_sw` at all - while asking it to emit one by itself fails
in the backend with `Cannot select: LGGPUISD::PREFIX_TP4`.  So a prefix is only
selectable fused to the operation it defines, which is exactly what the paragraph above
says about the literal.  The names (`tp4`, `tp8`) are not used to guess the effect, per
this chapter's rule.

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

* The jump-stack predicate's ENCODING half is settled and verified in both carriers:
  `jpush` and `jpop` carry `PJ2` as the predicate and `RA8` as the scalar operand, and
  the two opcodes differ only in `0x40000`, the bit the driver declares
  (`jumpstack_check.py`, problems 0).  What stays open is what the predicate MEANS -
  whether it gates the push or pop itself or selects a stack - and that needs a device.
* Nothing on the namespace itself, which is settled (2026-10-13): the identifiers are a packed launch-state record addressed through the kernarg pair `s[0:1]`, plus the local ids in `v0/v1/v2` and the group ids at `s[kernel_sgpr_cnt + 0..2]` - see the Compute page. What stays open is `getreg`'s own `hw_reg` numbering, which no producer path reaches.
* The defined prefix operations and the effect of their literals.
* The transfer granularity and address form of the shared-memory copies.
* The `flag` and `rfe` forms.
* Whether `check` may be placed inside divergent code, and what a zero
  domain operand orders.
* Whether the barrier has any effect outside a compute workgroup.
