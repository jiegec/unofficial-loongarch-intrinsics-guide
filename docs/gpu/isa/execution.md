# Execution

This chapter defines lane selection, the mask and condition registers, branch
targets and ordering. Encodings are in [Encoding](encoding.md) and the selector
space these instructions draw on is in [Operands](operands.md).

## Mask selector

A gated instruction carries a 2-bit selector, PJ2. Its position depends on the
machine format:

| Format | PJ2 position |
|--------|--------------|
| `vop` | `INST_HI[13:12]` |
| `vop-imm`, `flat`, `tex-buffer`, `shared-memory`, `jump-stack` | `INST_HI[17:16]` |
| `sio-id`, `sio-imm` | `INST_HI[21:20]` |
| `sop`, `sop-imm-control`, `smem`, `prefix` | none |

PJ2 = k selects `pk`, so 0 selects `p0`. The field is not an operand and is not
printed with the mnemonic. The selected mask is an absolute lane set: it is not
intersected with `p0` or with the mask that was live before it. A format without
the field cannot be gated, and a non-zero selector on such an instruction is an
encoding error, not a silent fall back to `p0`.

## Gating

PJ2 gates the vector, flat, buffer, image and shared-memory families, including
the shared-memory loads, stores and atomics. The scalar formats have no field and
execute wave-wide. A gated store writes only the selected lanes and a gated load
updates only the selected destinations. A gated instruction reads the selected
register and does not define it; only an instruction that names `pk` as a
destination, or a clobbering memory access, changes a mask register.

UNRESOLVED: what an unselected lane holds after a gated write. The sources state
both that the excluded lanes keep their previous value and that a gated vector
write clears them (`v_mov_b32` writing zero); a gated load is said to suppress
its update rather than clear, which does not settle the write case. Consequence:
a compiler MUST NOT build a merge from complementary gated writes, and MUST use a
per-lane select with the mask as an operand (`v_select_b32`, `d = c ? A : b`).

## The mask registers

`p0..p3` occupy the scalar selector numbers `0x7c..0x7f`. They are ordinary
scalar registers with distinguished numbers.

| Selector | Register | Content at wave entry |
|---------:|----------|-----------------------|
| `0x7c` | `p0` | the launch lane mask |
| `0x7d` | `p1` | UNRESOLVED |
| `0x7e` | `p2` | undefined |
| `0x7f` | `p3` | undefined |

A mask is one 32-bit word with one bit per lane, so one register covers one wave
of 32 lanes; the sources do not establish a wider wave. A shader MUST capture the
launch mask from `p0` before it writes `p0` or executes a buffer access: nothing
else records which lanes were active at entry. Keeping the live mask in `p0` is a
convention, not a requirement, because PJ2 names any of the four.

A vector compare writes `RD8`: a mask register, or an SGPR holding the 32-bit
lane mask. `s_wqm_b32` widens each non-zero quad to `0xf`; `s_quadmask_b32`
reduces each to one bit in bits 0..7, so the two are not inverses.

Any instruction whose scalar destination names `0x7c..0x7f` writes that
register. `s_mov_b32` copies a mask as an ordinary scalar irrespective of the
mask in effect, so a mask saved to an SGPR and reloaded later is the same lane
set. A buffer access redefines the mask registers without naming one. Which other
classes do the same is UNRESOLVED: one source keeps the masks live across flat,
atomic, shared-memory and image accesses, while a wider hazard rule counts image
and flat accesses as clobbering. A shader that needs a mask across any of those
MUST re-establish it.

## Condition register

SCC is a single system register at selector `0x80`, outside both ordinary
register files. Only one SCC value can be live at a time: a consumer MUST sit
between its producer and the next SCC write.

The immediate compare family writes SCC implicitly. Elsewhere SCC is an ordinary
selector: `s_addc_u32` and `s_subb_u32` take their carry or borrow from a
selected register, `s_select_b32` selects on one, and `cb`/`czb` test one.
Naming SCC is a choice, not an implicit input.

UNRESOLVED: the value a writer leaves. One family's reference gives `0xffff` for
true, the immediate family's gives 1. Consumers test zero against non-zero and
MUST NOT compare SCC with a particular non-zero value.

## Branches and the program counter

An instruction is two dwords, eight bytes. A branch carries a signed 32-bit
displacement in `INST_LO[31:0]`, counted in eight-byte instructions from the
instruction after the branch:

```text
target = address(branch) + 8 + displacement * 8
```

It spans -2^31..2^31-1 instructions. There is no delay slot and no NOP padding
after a branch.

| Mnemonic | Condition | Taken when |
|----------|-----------|------------|
| `b` | none | always |
| `cb` | selector in `INST_HI[7:0]` | the 32-bit condition is non-zero |
| `czb` | selector in `INST_HI[7:0]` | the 32-bit condition is zero |

The condition selector may name an SGPR, an inline constant, a mask register or
SCC. A mask condition is one 32-bit value, so the branch is a wave-wide decision
taken when any bit is set, not a per-lane branch. That is why divergence uses a
narrowed mask and an unconditional branch. A branch terminates its block.

`jpush` and `jpop` push and pop a control-flow record on a jump stack and both
carry PJ2; `jirl` writes a return position to a scalar pair and jumps to a pair
plus a displacement, and `getpc` writes a control-flow position to a pair.
UNRESOLVED: whether PJ2 gates `jpush` and `jpop`, and which
instruction's position the `jirl` and `getpc` pairs hold, in byte or instruction
units.

## Divergence

A divergent region is a mask value, not a separate control-flow state. Entering a
region narrows the mask, every instruction in the region selects the narrowed
register, and leaving the region restores the parent mask. With P the parent
mask, cond the branch condition and r the lanes a break removes:

```text
if:       then = P & cond        else = P & ~cond        merge = P
loop:     header = P             exit = P                 back edge = latch mask
break:    V' = V & ~r            for every value V still live
uniform:  control flow changes, the mask does not
```

Both arms of a divergent `if` lie on the wave's path, so every instruction of an
arm is reached even when no lane in its mask is active. A value written in an arm
and read after the merge MUST be rebuilt at the merge, with a `v_select_b32` per
destination or a full-width rewrite once the parent mask is restored. A value
only the arm reads needs no merge: every read of it runs under the same mask.

Four mask values can be resident at once; a value that cannot get a register is
copied into one before each instruction it gates, at one `s_mov_b32` per gate. A
compiler SHOULD keep at most four values live in a block.

## Ordering and barriers

`check` carries its domain in `INST_LO[3:0]`:

| Bit | Domain |
|----:|--------|
| 0 | kernel and scalar |
| 1 | vector memory, read |
| 2 | vector store, write |
| 3 | shared memory |

The bits combine; `0xf` names all four and is what a producer SHOULD use when the
pending class is unknown. `check` orders work already issued on the executing
path. It is not a lane rendezvous or a host fence, and a `check` before a
narrowed region does not order work issued inside it. It has no PJ2, so it cannot
be gated: the mask decides which lanes reach it, not what it orders.

`flag` has the same operand layout but updates execution-domain state, including
the memory (VM) selector, instead of ordering. `barrier` has no operand and no
PJ2. It synchronises the participating invocations of one workgroup and orders
the memory they share, and it is not a substitute for `check` on memory the
workgroup does not share.
