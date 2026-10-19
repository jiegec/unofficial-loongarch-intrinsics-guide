# Stage Conventions

The stage launcher copies or exposes record roots through stage-defined entry
registers. These are shader ABI conventions, not generic register allocation.
Each section below gives the live-in contract and the exit sequence of one
stage.

## DS as ordinary vertex

| Live-in | Contract |
|---------|----------|
| `v0` | Vertex/fetch index in the ordinary vertex convention |
| `v1` | Instance identifier when instancing is enabled |
| `s4:s7` | Constant-buffer descriptor at `const_buffer + 0x00` |
| `s8:s9` | Attribute descriptor-table root; entry `n` is at `s8:s9 + 16*n` |
| `s10` | Optional vertex-index bias in the biased-index convention |

The vertex output uses SIO0. The standard position record starts at offset
`0x0000`; the first full four-component varying starts at `0x0200`. The
examples use the assembled SIO operand order (stream identifier, predicate,
data tuple, channel count, offset, channel rotation):

```text
siowrrot_imm_c4 SIO0, p0, position, channum(4), 0x0000, chan(0)
siowrrot_imm_c4 SIO0, p0, varying0, channum(4), 0x0200, chan(0)
sioexport SIO0, p0, channum(8)
check
exit
```

## PS / fragment

| Live-in | Contract |
|---------|----------|
| `s4:s7` | Constant-buffer descriptor at slot 0 for buffer access |
| `s6:s7` | Resource root used by image loads at `+0x00` and sampler loads at `+0x30` |
| `s9` | Interpolation coefficient-table input for `load_sm_interp` |
| `v2:v3` | Barycentric pair for the default smooth interpolation form |
| `v12` or `v12:v15` | Feature-selected front-facing or fragment-coordinate inputs |

Overlapping register names represent alternative entry shapes, not two
independent values magically present at once. A pipeline that consumes both a
constant descriptor and an image root must define the hydration sequence and
the live range of each value.

Smooth interpolation consumes coefficient slot `4*attribute + component`:

```text
load_sm_interp p0, coefficient, s9, slot
check
v_interp_1_f32 p0, v2, coefficient, result
v_interp_2_f32 p0, v3, coefficient, result
```

PS color 0 uses SIO1, writes its four channels, then exports four channels:

```text
siowrrot_imm SIO1, p0, color.x, channum(4), 0x0000, chan(0)
... channels 1..3 ...
sioexport SIO1, p0, channum(4)
check
exit
```

## GS / geometry

The geometry stage reads a window of input vertices from the DS ring, writes
one or more varying records per output vertex, and emits vertices. The GS SIO
forms have stage-specific operand conventions.

### Input window

| Form | Contract |
|------|----------|
| `siordrot_id_c4` | `X1_1` selects the explicit-identifier form; `X2_16` is the 16-bit stream identifier/offset, `VA8` is the ring-channel offset, `VK8` is the input-vertex selector, and `X0_12` is the DS ring channel count |
| `siordrot_id_c4` result | `VJ8x4` is the four-register payload that receives the input window by read order; it is not a general arbitrary destination selector |

A primitive's input window is read before any output is written. The window
layout (channel count per vertex, varyings per vertex) is fixed by the DS ring
contract and the GS stage-config input IDs.

### Multi-varying output

An output vertex carries one position record and one or more varying records.
Each record is written to the SIO1 channel slots in order:

```text
siowrrot SIO1, p0, varying0.x, slot0, channum(1), 0x0000
siowrrot SIO1, p0, varying0.y, slot1, channum(1), 0x0000
siowrrot SIO1, p0, varying1.x, slot2, channum(1), 0x0000
... (one write per component per varying) ...
```

The number of varyings and their channel slots are fixed by the stream layout
before the emit sequence; `VK8` (shown here as `slot0`/`slot1`)
selects the channel slot and the encoded offset/rotation places the component.

### Multi-emit

A primitive is emitted with one `v_emit2` / `v_emit3` per output vertex. A
triangle therefore emits three vertices: `v_emit3` continues the strip,
`v_emit2` terminates the primitive.

```text
write_varyings(vertex0)
v_emit3 p0, emit_index, emit_state, chunks   ; emit vertex 0, continue strip
write_varyings(vertex1)
v_emit3 p0, emit_index, emit_state, chunks   ; emit vertex 1, continue strip
write_varyings(vertex2)
v_emit2 p0, emit_index, emit_state, chunks   ; emit vertex 2, terminate
```

| Form | Contract |
|------|----------|
| `v_emit2` / `v_emit3` | `GK11` is the emit-index VGPR, `GA11` the SGPR emit state, and `VJ8` the emit chunk count; stream control is supplied in those G/V positions |
| Final export | The stream header is updated with the emitted-primitive count before SIO export |

The emit count, the stream identifier, and the emitted-primitive count are
pipeline state, not instruction immediates. The SIO file semantics are not
fully determined (see [SIO](sio.md)).

## SS / streamout

| Live-in | Contract |
|---------|----------|
| `s0:s1` | `rw_buffer` descriptor-table root |
| `memory[s0:s1 + 0x20]` | Streamout buffer descriptor at slot 2 |
| Shader-info argument 2 | Stage record-count limit derived from configured byte extent and stride |

The SS shader stores through the descriptor at `W + 0x20`; the DS/GS producer,
streamout state, descriptor byte extent, and SS record limit must describe the
same output layout.

## VS and HS

VS and HS have independent records and enable bits. This revision does not
assign a standalone VS/HS live-in map or type value. A compiler must provide an
explicit stage contract rather than relabeling DS-as-VS registers.

## Summary

| Stage | Slot | Type | Entry registers | Exit |
|-------|------|-----:|-----------------|------|
| DS | `0x600` | `3` | `v0`, `s4:s9` | SIO0 position/varyings + `sioexport` |
| PS | `0x720` | `5` | `s4:s9`, `v2:v3`, `v12..` | SIO1 color + `sioexport` |
| GS | `0x660` | `4` | ring window via `siordrot_id_c4` | SIO1 varyings + `v_emit2/3` + export |
| SS | `0x6c0` | `6` | `s0:s1` root | stores through `W + 0x20` |
| VS/HS | `0x540/0x5a0` | unassigned | unassigned | unassigned |
