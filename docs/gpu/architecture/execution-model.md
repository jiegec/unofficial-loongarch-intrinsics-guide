# Execution Model

A shader executes as a **wave**: a group of lanes that share one instruction
stream and one set of scalar and mask registers. This chapter defines the wave
and its register files, the mask that gates each instruction, how a stage starts
and ends, and the synchronisation scopes a shader can express. Instruction
encodings and per-instruction semantics are in
[Execution](../isa/execution.md).

![Wave, lanes and registers](../assets/fig-wave.svg)

The figure shows one instruction issued for all 32 lanes, a vector file with one
copy per lane, a scalar and mask file with one copy per wave, and the selected
mask deciding which lanes the instruction acts on.

## Wave and lane

A wave is the unit at which control flow, masks and scalar state exist. It holds
up to 32 **lanes**; each lane runs one invocation and has its own vector
registers. Every lane executes the same instruction at the same time.

The supported width is 32 lanes. A lane mask is one 32-bit word with one bit per
lane, so one word covers exactly one wave and a 33rd lane has no representation.
A subgroup maps to one wave, and the supported subgroup size is 32. For a compute
dispatch the kernel descriptor also records the workgroup's wave count,
ceil(invocations / 32), in five bits at 31:27 of its second resource word.

These facts fix the ABI and the mask format, not the machine. They do not show
that a vector unit cannot run a wider wave: whether 32 is a hardware maximum is
UNRESOLVED. They do establish that a workgroup is a set of invocations, not a
wave. A workgroup with more than 32 invocations occupies more than one wave, and
only the workgroup barrier synchronises those waves.

That field is five bits, so 31 is the largest count it can hold. A workgroup of
1024 invocations is 32 waves, which does not fit and writes zero; whether the
hardware reads that as 32 waves or rejects the launch is UNRESOLVED. How many
waves a scheduler keeps resident, how many it issues per cycle, and the
per-instruction latency are UNRESOLVED.

## Register files

| File | Size | Copies | Holds |
|------|-----:|--------|-------|
| Scalar, `s0..s101` | 102 × 32 bits | one per wave | uniform values, pointers, descriptors, conditions |
| Vector, `v0..v255` | 256 × 32 bits | one per lane | per-invocation data |
| Predicate, `p0..p3` | 4 × 32 bits | one per wave | one lane mask each, bit *i* = lane *i* |
| Condition, `SCC` | 1 bit | one per wave | scalar compare result, carry and borrow |

The scalar file is uniform: every lane reads the same `sN`. The vector file is
private: two lanes may hold different values in the same `vN`. A predicate
register holds one bit per lane, so it is per wave, not per lane.

`p0..p3` are addressed in the scalar operand space but are not part of the 102
ordinary scalar registers: their numbers are `0x7c..0x7f`. They are legal
scalar operands, so a scalar move can copy `p0` into an SGPR and a scalar
logical operation can write a predicate register. `SCC` uses the scalar
selector `0x80`.

The launch fills stage-specific windows, tabulated in
[Registers and allocation](../shaders/registers.md): group id in `s11..s13`,
local invocation id in `v0..v2` and a kernarg pointer in `s6:s7` for compute,
which has no constant root; vertex id in `v0` and instance id in `v1`; and
interpolated inputs in the low vector registers for a fragment stage. A graphics
stage reads a constant root in `s4:s5`, and a fragment stage also reads its
resource root in `s6:s7`.

## The mask selector

Most instructions carry a 2-bit selector, **PJ2**. Selector value *k* runs the
instruction under `pk`. Eight of the twelve machine formats carry it:

| Machine format | PJ2 position |
|----------------|--------------|
| `vop` | `INST_HI[13:12]` |
| `jump-stack`, `vop-imm`, `tex-buffer`, `flat`, `shared-memory` | `INST_HI[17:16]` |
| `sio-id`, `sio-imm` | `INST_HI[21:20]` |
| `sop`, `sop-imm-control`, `smem`, `prefix` | no field |

The four formats without a field cannot be gated: a scalar ALU write is always
wave-wide, and a scalar load moves one value for the whole wave. The jump-stack
forms `jpush` and `jpop` carry a selector, but what it gates is UNRESOLVED.

The selected mask is an **absolute** lane set for that one instruction. It is
not intersected with `p0`: PJ2 = 1 runs under `p1` alone even when `p0` is
narrower. This is what lets several region masks stay live at once.

A gated instruction issues only for its selected lanes, and a gated store performs
no memory access for an unselected lane. What happens to the unselected lanes of a
destination register is not established: either the write is suppressed and those
lanes keep their previous value, or the write covers the whole register and zeroes
them. The difference appears at a region join, so a merge built from complementary
gated copies is valid only under suppression, which is not established. The
hardware behaviour is UNRESOLVED.

Do not build a merge from gated writes. Use one per-lane select whose mask
operand is the region's mask register (`v_select_b32`): it reads the mask as a
per-lane value, so each lane gets the correct source whatever a gated write would
have left behind.

A mask is a value, not a mode. A compare writes a 32-bit lane mask to a scalar
register: an SGPR destination materialises the mask as a value, while a
predicate-register destination writes the mask register itself and changes the
mask for the following instructions. The file holds four registers, so code with
more live masks than that keeps the values in SGPRs and copies one in
immediately before the instructions it gates.

At wave start `p0` holds the launch's active lanes and `p1..p3` are undefined.
A shader MUST write a predicate register before selecting it.

## Instruction execution

A shader is a sequence of 64-bit instructions, each two 32-bit dwords. The wave
fetches and issues each instruction once for all lanes; per-lane operands come
from each lane's vector registers, and a scalar operand is read once for the
wave.

Branches are wave-wide; there is no per-lane program counter and no delay slot.
A plain branch encodes a signed 32-bit instruction offset in `INST_LO[31:0]`,
in units of one 8-byte instruction from the instruction after the branch:

```text
target = branch_address + 8 + offset * 8
```

`cb` and `czb` add an 8-bit condition selector in `INST_HI[7:0]`. The condition
may be an SGPR, a predicate register, or `SCC`; a branch on a predicate register
tests the mask as a value, not per lane. The branch is taken for the whole wave
or for none of it.

## Stage entry

The launch gives the wave its entry address and its initial lane mask. For a
compute dispatch the address is the kernel descriptor's 64-bit entry field at
`+0x20`.

The prologue copies `p0` into a scalar register before anything changes the
mask. That copy is the wave's **coverage** mask: the lanes the stage may write.
`p0` is the only predicate register with a defined value at entry.

A fragment stage that executes a texture instruction, a derivative, a demote, or a
helper-invocation query widens `p0` to whole quads after the copy; a quad is four
adjacent lanes.

```text
s_mov_b32  exact, p0        ; the lanes that may write
s_wqm_b32  p0, exact        ; widen to every quad that holds an active lane
```

`s_wqm_b32` expands every nonzero group of four adjacent bits to all ones, so
lanes that may not write still execute as **helper** lanes and can supply
derivative operands; `s_quadmask_b32` reduces each group of four back to one
bit. The exact mask records which lanes may still write. A stage that can discard
or demote keeps it in `s12`, an ABI convention rather than a hardware slot, so a
terminate can update it in place. Before the export of a stage that uses helper
lanes, `p0` is set to coverage AND exact.

## Stage exit

A graphics stage that writes output ends in this order:

1. write the outputs: SIO writes for vertex, geometry and fragment values,
   stores for memory outputs;
2. order those writes with `check`;
3. commit the exported channels with one `sioexport`;
4. order the committed stream with `check`;
5. terminate with `exit`.

A compute stage has no export step: it orders its stores and exits.

The channel rules are in [SIO](../shaders/io.md#export). A vertex stage exports
position plus varyings; a fragment stage exports colour plus one when it writes
depth, and one channel even when it writes nothing; a vertex stage fused into a
geometry stage does not export. `sioexport` carries a 12-bit channel count in
`INST_HI[11:0]`, a stream select (SIO0 or SIO1) in `INST_HI[22]`, and its own
PJ2 selector in `INST_HI[21:20]`.

`check` carries four domain bits in `INST_LO[3:0]`: bit 0 kernel and scalar,
bit 1 vector memory, bit 2 vector store, bit 3 shared memory. Domain 0xf orders
all four classes. It does not signal completion and is not a host fence. `exit` has
no operands and no selector, so it cannot be applied to a subset of lanes: it
ends the stage for the wave.

A **discard** is not an exit. `terminate` writes zero to `p0`; `terminate_if`
clears only the killing lanes. Both also remove those lanes from the exact mask,
so the later export writes nothing for them, and both leave the wave running
under the reduced mask. Terminate is a fragment-stage operation. Because it
removes its lanes from `p0` and from the exact mask, those lanes can neither
write nor serve as helpers afterwards: every derivative and texture instruction
that needs them MUST precede the first terminate.

A compute stage that used a workgroup barrier closes with a full-domain `check`
before its `exit`. The first workgroup barrier is fronted with a full-domain
`check` and an extra `barrier` word; whether the extra word is required is
UNRESOLVED.

## Divergence

Lanes of one wave that take different paths are expressed as masks; no lane has
its own program counter. A region is entered by narrowing the live mask and left
by restoring the saved one. A value whose lanes are read outside the region it
was written in needs a per-lane select at the join, with the region mask as the
selector source; it is not rebuilt from gated copies. A branch on a predicate
register skips a region when no lane needs it, because the mask is tested as a
value.

A fragment wave tracks two masks:

| Mask | Meaning |
|------|---------|
| Coverage | the lanes the launch made active, copied from `p0` at entry |
| Exact | coverage minus the lanes that discarded or demoted |

When the stage uses helper lanes, `p0` runs widened to whole quads while the
exact mask stays narrow, so `is_helper_invocation` is `p0` AND NOT (coverage AND
exact): a lane that executes but may not write. At export `p0` is set to
coverage AND exact, so demoted lanes cannot write colour or depth.

> UNRESOLVED: whether whole-quad widening applied to a region mask, rather than
> to the coverage mask, keeps the region's lanes exclusive in every instruction
> family.

## Synchronisation scopes

| Scope | Mechanism | Limit |
|-------|-----------|-------|
| One invocation | instruction order; `check` for pending memory work | `check` orders this wave's pending work, not another wave's |
| One wave | one instruction stream; lanes issue together | no instruction needed; a subgroup is one wave |
| One workgroup | `barrier` for execution, `check` for memory | `barrier` has a fixed workgroup scope and no operand |
| Across workgroups | none in the instruction set | a shader cannot order another workgroup's accesses |
| Between submissions | host ordering | outside the shader, see [Submissions](../host/submissions.md) |

`barrier` synchronises the participating lanes of one workgroup and orders
workgroup-visible memory. A workgroup that spans several waves needs it to
rendezvous those waves; within one wave the lanes already issue together.
`check` orders the pending work named by its domain bits for the invoking wave;
it is not a lane rendezvous. Neither instruction signals completion, and neither
is a host fence.

The control instructions carry no execution-scope operand: `barrier` is fixed at
workgroup scope, and `check` names memory domains, not scopes. A shader
therefore cannot name a cross-workgroup rendezvous; a dispatch that needs one
must be split so that the host orders the two submissions.

## Limits and open questions

| Property | Value |
|----------|-------|
| Wave width | 32 lanes (supported ABI width; not proven as a hardware maximum) |
| Scalar registers | 102, `s0..s101` |
| Vector registers | 256 per lane, `v0..v255` |
| Predicate registers | 4, `p0..p3`, at scalar numbers `0x7c..0x7f` |
| Condition register | `SCC`, scalar selector `0x80` |
| Instruction size | 64 bits, two dwords |
| Formats with a mask selector | 8 of 12; `HI[13:12]`, `HI[17:16]` or `HI[21:20]` |
| Branch displacement | signed 32 bits, in 8-byte instruction units |
| Wave-count field | 5 bits, bits 31:27; 31 is the largest count |

Open items, each stated above: the hardware maximum wave width, the behaviour of
a 32-wave workgroup, the fate of a vector destination outside a gated write,
whole-quad widening of a region mask, the extra barrier word, the jump-stack
selector, and resident waves, issue width and latency.
