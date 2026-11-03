# Registers and Allocation

This chapter defines the register files, the registers a launch supplies and the
allocation constraints a compiler must respect. The record fields are in
[Shader ABI](shader-abi.md); the operand encoding is in
[Operands](../isa/operands.md).

## Register files

| File | Size | Selector | Notes |
|------|-----:|----------|-------|
| Scalar | 102, `s0..s101` | `0x00..0x65` | One copy per wave. |
| Vector | 256, `v0..v255` | `0x100 + n` | One copy per lane. |
| Lane mask | 4, `p0..p3` | `0x7c..0x7f` | One bit per lane. |
| Condition | `SCC` | `0x80` | One bit per wave. |

## Reserved scalar window

The low twelve scalar registers are reserved for the launch and the lane-mask
window:

| Registers | Reserved for |
|-----------|--------------|
| `s0..s3` | Lane-mask results. |
| `s0..s11` | Vertex descriptor and vertex inputs, when used. |
| `s0..s7` | Fragment and geometry descriptors, when used. |

A compiler reserves the whole window rather than the part its stage reads.

## Launch registers

| Register | Contents | Stage |
|----------|----------|-------|
| `s0:s1` | Read-write buffer root. | Streamout |
| `s2` | Fragment interpolation selector. | Fragment |
| `s4:s5` | Constant root. | Vertex, geometry, fragment |
| `s6:s7` | Image and sampler descriptor root. | Vertex, geometry, fragment |
| `s8:s9` | Vertex attribute descriptor table. | Vertex |
| `s9` | Fragment input window base. | Fragment |
| `v0` | Vertex index, with the draw's vertex offset applied. | Vertex |
| `v1` | Instance identifier. | Vertex |
| `v0` | Packed input-vertex selectors, six 5-bit fields. | Geometry |
| `v1` | Primitive identifier. | Geometry |
| `v0:v1` | Barycentric pair when a sample mask is used. | Fragment |
| `v2:v3` | Barycentric pair, smooth interpolation. | Fragment |
| `v6` | Coverage or sample mask. | Fragment |
| `v8:v9` | Barycentric pair, noperspective interpolation. | Fragment |
| `v12..v15` | Fragment coordinate. | Fragment |
| `v12` or `v16` | Front-facing flag. | Fragment |

A launch value is read, not written. The register files are not otherwise
pre-loaded.

## Compute launch registers

| Register | Contents |
|----------|----------|
| `s[6:7]` | Kernarg pointer. |
| `s11..s13` | Group id X, Y, Z. |
| `v0..v2` | Local invocation id X, Y, Z. |
| `s0..s5` | Ordinary allocatable scalar registers; no constant root. |

The group-id window starts after the kernel's declared SGPR count. The compiler
reads `s[kernel_sgpr_cnt + 0]`; the launch declares 11. A kernel that reports
fewer SGPRs than the window would leave the group ids outside the count it
reports.

## Size rules

The record's size fields are the allocation a stage asks for, not the
architectural maximum. The rules are in [Shader ABI](shader-abi.md#size-rules).
The vector file is 256 registers; the field that carries its size encodes 256 as
255.

## Geometry window

Geometry output uses a window of vector registers for the SIO1 record. The
current compiler reserves `v33` for the window counter, `v35` for the separator,
`v19` for the final separator copy and `v45` for the emit index. These are
compiler assignments, not launch registers.

Window maintenance must preserve the constant root in `s4:s5` across later
uniform loads. A scalar register reserved for a launch descriptor is not
available as execution-mask scratch merely because an earlier load has finished.

## Allocation constraints

- A tuple base MUST be aligned to its width: a two-register tuple to an even
  register, a four-register tuple to a multiple of four, an eight-register tuple
  to a multiple of eight.
- A compare that writes a lane mask writes the mask register; a compare that
  writes a scalar register is not the established form.
- The condition register is short-lived across compares; a consumer must be
  materialised before the next compare.
- A lane-mask register is undefined at launch except for the live mask installed
  by the prologue.

## Open questions

- Whether an odd-base vector tuple is legal.
- Whether the lane-mask window and the descriptor scratch use of `s0..s3` alias.
- Whether a stage may use the part of the reserved window it does not read.
