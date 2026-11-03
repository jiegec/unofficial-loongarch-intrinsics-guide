# Registers and Allocation

Every stage receives a register file and a window of registers the launch has
already filled. This chapter defines the files, the reserved window, the entry
values and the granularity of the counts. The record that carries the counts is
in [Shader ABI](shader-abi.md); the selectors are in
[Operands](../isa/operands.md#general-source-selector).

## Register files

| File | Size | Selector | Scope | Holds |
|------|-----:|----------|-------|-------|
| Scalar `s0..s101` | 102 × 32 bits | `0x00..0x65` | one per wave | uniform values, pointers, descriptors, masks |
| Vector `v0..v255` | 256 × 32 bits | `0x100 + n` | one per lane | per-invocation data |
| Lane mask `p0..p3` | 4 × 32 bits | `0x7c..0x7f` | one per wave | one lane mask each, bit *i* = lane *i* |
| Condition `SCC` | 1 bit | `0x80` | one per wave | compare result, carry, borrow |

The scalar file is wave-wide and the vector file is private to a lane. A wave is
32 lanes wide, so one lane mask covers one wave.

`p0..p3` are scalar registers at four distinguished selectors, not a fifth file.
`PJ2` names which of the four an instruction runs under, so four masks can be
live at once. A compare result is a 32-bit lane mask in an ordinary scalar
register, copied into `p0..p3` before it gates anything. `p0` holds the mask the
wave is running under, so a compare MUST NOT write it: an all-false result
retires every lane and the draw disappears.

`SCC` has its own selector. A scalar compare writes it and an add-with-carry
names it as a source, so a consumer MUST follow its producer with no
flag-setting instruction in between.

## The scalar window

Every stage but compute reads descriptor roots from the bottom of the scalar
file. `s9` appears twice because a stage reads only its own window.

| Register | Contents | Read by |
|----------|----------|---------|
| `s0:s1` | Read-write root, the record's RW Buffer field. | The streamout kernel only. |
| `s2` | Interpolation selector, the scalar source of the interpolation move. | Fragment, for a flat input or the primitive id. |
| `s4:s5` | Constant root. | Vertex, geometry, fragment. |
| `s6:s7` | Resource and sampler root. | Any graphics stage that samples. |
| `s8:s9` | Attribute descriptor table, 16-byte records at `s8:s9 + 16 × n`, `n` = 0 to 15. | Vertex. |
| `s9` | Base of the coefficient window the fragment input ring reads. | Fragment. |

The read-write root is the only one a single stage reads. The streamout kernel
reads the transform-feedback descriptor at offset `+0x20` in that root; no
vertex, geometry or fragment shader may use the pair as a root, because those
registers are ordinary targets for lane-mask results. A draw that runs no
streamout kernel points the field at a mapped page and never dereferences it.

The launch fills these registers before the first instruction runs, so a compiler
MUST keep them out of allocation; a temporary placed there would be read back as
a descriptor base. Which slots each stage withholds depends on what the shader
uses:

| Stage | Slots withheld | Pool | When |
|-------|----------------|------|------|
| Vertex | `s0..s11` | ordinary | The shader reads a uniform block, a storage block, a vertex input or a sampled image. |
| Fragment, geometry | `s0..s7` | ordinary | The shader reads a uniform block, a storage block or a sampled image. |
| Vertex, fragment | the lane-mask window `s0..s3` | lane mask | The shader produces lane-mask values. |
| Geometry | the lane-mask window `s0..s11`, with `s0`, `s1`, `s2`, `s4..s7` and `s11` never handed out | lane mask | The shader produces lane-mask values. |
| Compute | the lane-mask window `s0..s10` | lane mask | The shader produces lane-mask values. |
| Compute | `s6`, `s7`, `s11..s13`, `v0..v2` | ordinary, vector | Always. |

A compute shader that uses the packed dot product with an implicit scalar
selector also loses `s0`. Geometry needs the wider window because its emit path
writes `s0` and `s1` before every emitted vertex, uses `s2` as scratch and keeps
a save slot in `s11`. A withheld slot never raises the reported count, which is
the ordinary pool's high-water.

## Entry values: graphics

| Register | Contents | Stage |
|----------|----------|-------|
| `v0` | Vertex identifier, with the draw's base vertex already applied. | Vertex |
| `v1` | Instance identifier. | Vertex |
| `v0` | Six 5-bit input-vertex selectors, one per vertex of the primitive. Entry only. | Geometry |
| `v1` | Primitive identifier at entry; the invocation identifier when the record enables more than one invocation. | Geometry |
| `v0:v1` | Barycentric pair, sample-mask layout. | Fragment |
| `v2:v3` / `v4:v5` | Barycentric pairs, perspective and perspective centroid. | Fragment |
| `v8:v9` / `v10:v11` | Barycentric pairs, non-perspective and non-perspective centroid. | Fragment |
| `v2` / `v6` | Sample mask, without and with a coordinate read. | Fragment |
| `v12..v15` / `v2..v5` | Fragment coordinate, without and with the sample mask. | Fragment |
| `v12` / `v16` | Face flag, without and with a coordinate read. | Fragment |

The fragment stage has two layouts of the same registers. With no sample mask the
barycentrics start at `v2`, the coordinate takes `v12..v15` and the face flag
takes `v12`; with the sample mask every interpolation mode reads `v0:v1`, the
coordinate moves to `v2..v5` and the mask sits at `v6`. Reading both the
coordinate and the face flag reaches `v16`, so such a shader needs at least
seventeen registers.

The geometry entry values are consumed rather than preserved: the prologue copies
`v0` and the emit recipe reuses `v1`, so a geometry shader MUST capture both
before it emits.

The record's VGPR initial count is 4 for the vertex role, 2 for geometry (one
more with the primitive id), 15 for fragment and 2 for streamout. It is not the
register count, and the effect of the field is UNRESOLVED.

## Entry values: compute

A compute dispatch has no constant root and no stage descriptor window. The
launch fills three things:

| Register | Contents |
|----------|----------|
| `s6:s7` | Kernarg pointer. Every hidden-argument and descriptor address is an offset from it. |
| `s11..s13` | Group id X, Y, Z. |
| `v0..v2` | Local invocation id X, Y, Z, per lane. |

The group window starts at the scalar count the kernel descriptor declares, not
at the count the shader reports. The descriptor declares 11 kernel scalar
registers and 3 group registers, so group id X is `s11`. The reported count is
the ordinary allocatable high-water and excludes both the kernarg pair and the
group window.

## Allocation granularity

| Path | Scalar | Vector |
|------|--------|--------|
| Graphics record | At least 16, at most 102, not rounded. | At least 8 vertex, 64 geometry, 16 fragment, 2 streamout; rounded up to four. 256 is encoded as 255. |
| Compute kernel descriptor | `ceil(sgpr / 8)` in bits 9:6 of the word at `+0x30`; at most 120. | `ceil(vgpr / 4)` in bits 5:0; at most 252. |

A graphics stage also raises its vector count: a geometry stage exporting two or
three varyings needs at least 120 or 168 registers, and a fragment shader that
reads both the coordinate and the face flag needs at least 20.

A count beyond the limit is rejected rather than truncated. The descriptor's
scalar ceiling of 120 is above the file's 102 and never binds; its vector ceiling
of 252 is below the file's 256, so such a kernel cannot launch.

A multi-register operand occupies a consecutive run whose base MUST satisfy the
run's alignment; an unaligned run is rejected rather than split. Tuple alignments
are in [Descriptors](descriptors.md#alignment) and
[Operands](../isa/operands.md#tuple-alignment).

## Open questions

- The contents of `s10:s11` at a vertex launch: the window withholds them and no
  stage of the supported pipeline reads them.
- Whether a compute kernel's reported scalar count must cover `s11..s13`. The
  descriptor declares the group window separately, and a driver MUST NOT assume
  the reported count backs those registers.
- Whether the geometry stage receives the primitive identifier and the invocation
  identifier in `v1` at once. Both are read from `v1`, and a record can enable
  both.
- What the interpolation move does with its selector operand. `s2` is the
  established source for flat inputs and for the primitive id; the effect of the
  operand is not established.
- Whether a fragment shader that reads both the sample mask and a centroid input
  really uses `v0:v1`. The compiler does, and no independent evidence fixes it.
