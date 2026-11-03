# Operands

This chapter defines the operand classes, the selector encodings, the register
files and the modifier kinds. The instruction formats are in
[Encoding](encoding.md); the execution model is in [Execution](execution.md).

## Operand classes

| Class | Fields | Meaning |
|-------|--------|---------|
| Vector | `VJ8`, `VK8`, `VA8`, `VQ8`, `VD8` | A VGPR index, or a row-defined zero. |
| General source | `GJ8`, `GK8`, `GJ11`, `GK11`, `GA11` | A selector from the table below. |
| Scalar or descriptor | `RJ8`, `RK8`, `RA8`, `RD8` and their tuples | A scalar register, descriptor base, result or lane-mask position. |
| Immediate | `IJ32`, `IK32`, `IA16`, `IJ16`, `IK16`, `UO8`, `UO12`, `UO20` | A literal or an offset. |
| Modifier | `M.*` | An operation or addressing control. |
| Predicate | `PJ2` | The instruction's lane-mask selector. |

A tuple suffix names consecutive registers: `RJ8x4` is four consecutive scalar
registers, `VJ8x8` eight consecutive vector registers. A tuple base is aligned
to its width.

## General-source selector

An 8-bit general-source field carries a base selector:

| Selector | Meaning |
|----------|---------|
| `0x00..0x65` | Ordinary scalar registers `s0..s101`. |
| `0x66` | Constant zero. |
| `0x67` | Constant zero, high form. |
| `0x68..0x77` | Special values with no established role; read zero. |
| `0x78..0x7a` | `pi`, `e`, `1/(2*pi)`; a second copy of the tail of `0xf0..0xfa`. |
| `0x7b` | Special value with no established role; reads zero. |
| `0x7c..0x7f` | Lane-mask registers `p0..p3`. |
| `0x80` | The condition register `SCC`. |
| `0x81..0x9f` | Special values with no established role; read zero. |
| `0xa0..0xdf` | Integer constants 1 through 64, encoded as `0x9f + n`. |
| `0xe0..0xef` | Integer constants -1 through -16, encoded as `0xdf + n`. |
| `0xf0..0xfa` | 0.5, -0.5, 1.0, -1.0, 2.0, -2.0, 4.0, -4.0, pi, e, `1/(2*pi)`. |
| `0xfb..0xff` | Special values with no established role; read zero. |

The texture-buffer format also carries `RA9x8`, a nine-bit descriptor base: the
low eight bits are in `INST_LO[15:8]` and the ninth is `INST_HI[11]`. The
corresponding operands are `RK8x4` and `RK9x8`.

The unmodelled ranges are not ordinary scalar registers. They MUST NOT enter a
scalar allocation pool, and a decoder SHOULD reject them. Their read value is
zero, which is the default outcome for an unmodelled selector and carries no
information about that selector.

## 11-bit selector

Vector ALU sources use an 11-bit selector:

```text
selector[10:0] = { namespace[2:0], base[7:0] }
GJ11 = { INST_HI[2:0], INST_LO[7:0] }
GK11 = { INST_HI[5:3], INST_LO[15:8] }
GA11 = { INST_HI[8:6], INST_LO[23:16] }
```

| Bit | Meaning |
|----:|---------|
| `base[7:0]` | The base selector. |
| `0x100` | Select a vector register: the source is `v[base]`. |
| `0x200` | Clear the sign bit of the source value. |
| `0x400` | Flip the sign bit of the source value. |

The modifier bits act on the value, not on the register. `0x600` is `-abs(x)`.
They are per source and distinct from the instruction's `M.mod2` and `M.clmp1`
modifiers.

## Register files

The register files are defined in [Registers and allocation](../shaders/registers.md).
Two encoding facts belong here:

- `p0..p3` have the fixed selectors `0x7c..0x7f`, and `SCC` has `0x80`, so both
  are reachable from any general-source position.
- A lane mask holds one bit per lane, and a wave is 32 lanes wide, so one
  register covers exactly one wave.

## Immediate forms

| Form | Width | Use |
|------|------:|-----|
| `IJ32`, `IK32` | 32 | Literal bit pattern in the low word. |
| `IA16`, `IJ16`, `IK16` | 16 | Identifier, channel or offset literal. |
| `UO8` | 8 | Unsigned offset. |
| `UO12` | 12 | Unsigned offset for buffer addressing. |
| `UO20` | 20 | Unsigned offset for scalar memory addressing. |

## Modifiers

A modifier is distinguished by what it acts on, not by its name:

| Kind | Acts on | Members |
|------|---------|---------|
| Enable | Whether another operand contributes. | `M.offen1`, `M.idxen1` |
| Width | How many registers another operand occupies. | `M.rsz1` |
| Form | Which instruction this is; fills no operand slot. | `M.na1` |
| Value | Its own immediate operand slot. | `M.dmask4`, `M.un1`, `M.bl11`, `M.bl21`, `M.mod2`, `M.clmp1` |
| Marker | Records a property of the instruction. | `M.vm1` |

| Field | Bits | Kind | Default | Meaning |
|-------|-----:|------|--------:|---------|
| `M.mod2` | 2 | Value | 0 | Vector operation mode. |
| `M.clmp1` | 1 | Value | 0 | Clamp the result. |
| `M.offen1` | 1 | Enable | 0 | Add the offset operand. |
| `M.idxen1` | 1 | Enable | 0 | Add the index operand. |
| `M.bl11`, `M.bl21` | 1 each | Value | 0 | Memory behaviour controls. |
| `M.un1` | 1 | Value | 0 | Unnormalized texture coordinates. |
| `M.na1` | 1 | Form | 0 | Array image. |
| `M.rsz1` | 1 | Width | 0 | Resource tuple width: eight or four dwords. |
| `M.dmask4` | 4 | Value | 0 | Texture channel mask. |
| `M.vm1` | 1 | Marker | 1 | Control instruction flag. |
| `M.sioid1` | 1 | Value | 0 | SIO stream selector. |
| `M.channum12` | 12 | Value | 0 | SIO window size in channels. |
| `M.chan12` | 12 | Value | 0 | SIO channel slot. |

An enable bit changes the instruction's operand count: clearing it removes the
operand rather than zeroing it. A width bit changes another operand's size. A
form bit is not an operand. A value bit is an ordinary immediate.

> UNRESOLVED: the exact arithmetic of `mod2` and `clmp1`, and the meaning of
> `bl1` and `bl2` to the memory system.

## Tuple alignment

A two-register tuple starts at an even register; a four-register tuple starts at
a multiple of four; an eight-register tuple starts at a multiple of eight. A
destination tuple follows the same rule.

> UNRESOLVED: whether an odd-base pair is legal.
