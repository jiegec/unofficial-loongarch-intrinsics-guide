# Registers and Allocation

This chapter is the allocation contract: the register files, what a launch
supplies, which registers a compiler may not use, and the size rules a shader-info
record must satisfy.  The record itself is in the [Shader ABI](abi.md); the
operand encoding that selects a register is in
[Operands and Registers](../isa/operands.md); the per-stage entry conventions are
in [Stage Interfaces](stages.md).

## The register files

| File | Extent | Selector | Notes |
|------|--------|----------|-------|
| Scalar | `s0..s101`, 102 registers | `0x00..0x65` | one copy per wave, shared by all lanes |
| Vector | `v0..v255`, 256 registers | `0x100 + n` | one copy per lane |
| Lane mask | `p0..p3` | `0x7c..0x7f` | one bit per lane, so one register is one wave wide |
| Condition | SCC | `0x80` | a single bit, not part of a pool |

A scalar register is shared by the whole wave and a vector register is private to
a lane, which is why the two files have different sizes and why a lane mask fits
a wave in one register.

## Reserved scalar registers

The low scalar registers carry values the launch or the shader's own prologue
supplies, so a compiler must not allocate them:

| Registers | Reserved for |
|-----------|--------------|
| `s0..s3` | lane-mask results |
| `s0..s11` | the vertex stage's descriptor and vertex inputs, when used |
| `s0..s7` | the fragment and geometry stages' descriptors, when used |

The allocation window at the bottom of the scalar file is therefore twelve
registers wide, covering the lane-mask slots and the graphics ABI inputs above
them.  Which of those twelve a given stage actually uses depends on the stage and
on the pipeline state; a compiler reserves the whole window rather than the part
its stage reads.

## Launch-supplied registers

The hardware provides these registers at stage entry, and a stage must not expect
any other register to be pre-loaded:

| Register | Contents | Stage |
|----------|----------|-------|
| `s0:s1` | read-write buffer root (record root 1) | streamout |
| `s2` | fragment interpolation selector | fragment |
| `s4:s5` | constant root (record root 3) | vertex, geometry, fragment |
| `s6:s7` | image and sampler descriptor root (record root 4) | vertex, geometry, fragment |
| `s8:s9` | vertex attribute descriptor table | vertex |
| `s9` | fragment input window base | fragment |
| `v0` | vertex index, with the draw's vertex offset already applied | vertex |
| `v1` | instance identifier | vertex |
| `v0` | packed input-vertex selectors: six 5-bit fields | geometry |
| `v1` | primitive identifier | geometry |
| `v0:v1` | the barycentric pair when a sample mask is used | fragment |
| `v2:v3` | barycentric pair, smooth interpolation | fragment |
| `v6` | coverage or sample mask | fragment |
| `v8:v9` | barycentric pair, noperspective interpolation | fragment |
| `v12..v15` | fragment coordinate | fragment |
| `v12` or `v16` | front-facing flag | fragment |

A launch value is read, not written: the inputs above are live at entry and a
shader that overwrites one before using it loses it.  The register files are not
otherwise pre-loaded.

## Size rules

The shader-info record's size fields must satisfy:

* SGPR size is at least 16 and at most 102.
* VGPR size is at least 8 for the vertex role, 64 for geometry, 16 for fragment
  (4 for the colourless coverage shape) and 2 for streamout, and is rounded up to
  a multiple of four otherwise.
* Code size must fit 16 bits, and a complete instruction stream is a multiple of
  eight bytes.

The size fields are the *allocation* a stage asks for, not the architecture
maximum; the vector file's 256 registers are the file size, and the field that
carries a size encodes 256 as 255.

## UNRESOLVED

* The meaning of the record's "VGPR initial count" field: the name says count, the
  value for the vertex role is 4 while the stage reads `v0` and `v1`, and the
  fragment value is 15 while `v16` is read in one configuration.
* Whether the lane-mask window and the descriptor scratch use of `s0..s3` alias,
  and whether a stage may use the part of the window it does not read.
* The hydration of `s10:s11`.
* Whether `s9` is simultaneously the attribute-table high half and the fragment
  input window base; the two roles are carried by the same register in different
  stages.
* Whether the launch supplies any value beyond the registers listed above.
