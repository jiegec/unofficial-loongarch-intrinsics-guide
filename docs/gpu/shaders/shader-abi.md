# Shader ABI

A stage is described by a 96-byte shader-info record in the state object. The
record tells the hardware where the code is, how many registers it needs, where
its descriptor roots are and what arguments it receives. This chapter defines
the record, the size rules and the entry and exit sequence.

## Shader-info record

| Bits | Field | Meaning |
|------|-------|---------|
| `[7:0]` | SGPR size | Scalar registers the stage uses. |
| `[15:8]` | VGPR size | Vector registers the stage uses; 256 is encoded as 255. |
| `[31:16]` | LDS size | Workgroup shared memory the stage uses. Zero for the graphics path. |
| `[39:32]` | Shader type | 3 vertex role, 4 geometry, 5 fragment, 6 streamout. |
| `[47:40]` | VGPR initial count | Stage launch value. |
| `[63:48]` | Code size | Byte count of the shader image. |
| `[127:64]` | Alignment padding | Not a field. |
| `[191:128]` | Kernel entry | GPU virtual address of the first instruction. |
| `[255:192]` | Kernel config | Zero in the current path. |
| `[319:256]` | Root 1 | Read-write buffer root. |
| `[383:320]` | Root 2 | Constant-buffer root; written but with no launch effect. |
| `[447:384]` | Root 3 | Constant root; hydrated into `s4:s5`. |
| `[511:448]` | Root 4 | Image and sampler descriptor root; hydrated into `s6:s7`. |
| `[575:512]` | Root 5 | Attribute descriptor root. |
| `[607:576]` | Argument 0 | Stage-defined 32-bit argument. |
| `[639:608]` | Argument 1 | Stage-defined 32-bit argument. |
| `[671:640]` | Argument 2 | Stage-defined 32-bit argument. |
| `[703:672]` | Argument 3 | Stage-defined 32-bit argument. |
| `[735:704]` | Reserved | Zero. |
| `[767:736]` | Tail | Streamout record capacity or zero. |

The record is 96 bytes. The arguments are four 32-bit fields, not two 64-bit
fields; the tail is a separate dword.

## Stage slots

The record is stored in one of six slots:

| Slot | Offset | Role |
|-----:|-------:|------|
| 0 | `0x540` | Prefetch copy of the vertex record. |
| 1 | `0x5a0` | Unused. |
| 2 | `0x600` | Domain / ordinary vertex. |
| 3 | `0x660` | Geometry. |
| 4 | `0x6c0` | Streamout. |
| 5 | `0x720` | Fragment. |

## Size rules

| Field | Rule |
|-------|------|
| SGPR size | At least 16; at most 102. |
| VGPR size | At least 8 for the vertex role, 64 for geometry, 16 for fragment (4 for the colourless coverage shape), 2 for streamout; rounded up to a multiple of four otherwise. |
| Code size | Fits 16 bits; a complete instruction stream is a multiple of eight bytes. |
| Kernel entry | The first instruction; no separate shader header. |
| LDS size | The graphics path writes zero. Compute uses the kernel descriptor instead. |

The VGPR size field encodes 256 as 255. A geometry stage that exports two
varyings raises its VGPR allocation to at least 120; three varyings raise it to
at least 168. These are floors, not ceilings.

The current compiler limits a binary to 8191 instructions, so the 16-bit code
size is not the limiting factor.

## Entry and exit

The kernel entry is the address of the uploaded code. Every stage begins by
capturing the active lane mask into a scalar register and, when it uses helper
lanes, installing a whole-quad mask.

Every stage ends in a fixed order:

1. write the stage outputs;
2. commit the export;
3. order pending memory and interface work;
4. terminate the invocation.

A discard restores the captured mask, clears the lanes that must not write and
continues; it does not execute the terminator.

## Upload

The shader binary is uploaded to a GPU buffer that is mapped readable and
executable. The allocation is aligned to a power of two and has one zeroed tail
page after the code, so an instruction prefetch beyond the last instruction
reads zero rather than an unmapped page. The code size in the record MUST equal
the binary length.

> UNRESOLVED: whether the instruction cache needs an explicit invalidate after
> a shader binary is replaced at the same address.

## Root hydration

The constant root is hydrated verbatim into `s4:s5` at a vertex or fragment
launch. The resource root is hydrated into `s6:s7`. The read-write root is used
by the streamout stage. The constant-buffer root has no launch effect; the
constant root supplies `s4:s5`.

## Open questions

- The meaning of the VGPR initial count: the name says count, the vertex value is
  4 while the stage reads `v0` and `v1`, and the fragment value is 15 while
  `v16` is read in one configuration.
- The hydration of `s10:s11`.
- Whether the LDS size field is honoured by any graphics path.
