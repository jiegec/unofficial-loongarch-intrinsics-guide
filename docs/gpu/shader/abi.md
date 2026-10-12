# Shader ABI

A shader stage is described by a *shader-info* record inside the state object.
The record tells the hardware where the code is, how many registers it needs,
and where its descriptor roots are.  This chapter defines the record, the
registers a stage receives at entry, and the sequence a stage must end with.

Values are those of the [LG200 profile](../chips/lg200.md); the record shape is
the same across the family.

## Stage slots

Six 0x60-byte slots start at state offset `0x540`:

| Slot | Offset | Shader type | Role |
|-----:|-------:|------------:|------|
| 0 | `0x540` | — | vertex; never written |
| 1 | `0x5a0` | no value assigned | hull; never written |
| 2 | `0x600` | 3 | vertex (the ordinary vertex role) |
| 3 | `0x660` | 4 | geometry |
| 4 | `0x6c0` | 6 | streamout |
| 5 | `0x720` | 5 | fragment |

The ordinary vertex role is carried by **slot 2 with the DS-as-vertex control
bit set**, not by slot 0.  A vertex shader therefore runs as a "domain" record
with shader type 3.  Slot 0 and slot 1 are never programmed, and there is no
shader type value for a hull stage.

## Shader-info record

96 bytes:

| Bits | Field | Meaning |
|-----:|-------|---------|
| `[7:0]` | SGPR size | scalar registers the stage uses |
| `[15:8]` | VGPR size | vector registers the stage uses; the value 256 is encoded as 255 |
| `[31:16]` | LDS size | always zero in every established path |
| `[39:32]` | Shader type | 3, 4, 5 or 6 |
| `[47:40]` | VGPR initial count | stage launch value: 4 for the vertex role, 2 for geometry (3 when a primitive identifier is needed), 15 for fragment, 2 for streamout |
| `[63:48]` | Code size | byte count of the shader image |
| `[127:64]` | — | alignment padding, not a field |
| `[191:128]` | Kernel entry | GPU VA of the first instruction |
| `[255:192]` | Kernel config | always zero in every established path |
| `[319:256]` | Root 1 | read-write buffer root |
| `[383:320]` | Root 2 | constant-buffer root; written but with no launch effect |
| `[447:384]` | Root 3 | constant root |
| `[511:448]` | Root 4 | image and sampler descriptor root |
| `[575:512]` | Root 5 | attribute descriptor root |
| `[639:576]` | Argument 0 | stage-defined |
| `[703:640]` | Argument 1 | stage-defined |
| `[767:704]` | Argument 2 and tail | streamout writes the record capacity here |

Size rules:

* SGPR size is at least 16 and at most 102.
* VGPR size is at least 8 for the vertex role, 64 for geometry, 16 for fragment
  (4 for the colourless coverage shape) and 2 for streamout, and is rounded up
  to a multiple of four otherwise.
* Code size must fit 16 bits, and a complete instruction stream is a multiple of
  eight bytes.

## Entry and exit

`kernel_entry` is the address of the uploaded code: it points at the first
instruction word, and there is no separate shader header or descriptor.

Every stage begins by capturing the active lane mask into an ordinary scalar
register, so that the mask can be restored.  With fragment helper-lane handling
the capture is followed by installing a full-quad mask.

Every stage ends in a fixed order:

```text
stage outputs           (SIO writes, or stores through descriptors)
sioexport               commit the exported channel count
check                   order pending memory and interface work
exit                    terminate the invocation
```

There are no separate "sio check" or "sio exit" instructions: `check` is the
ordering operation and `exit` is the terminator, and both are the ordinary
control-prefix forms described in
[the control flow and ordering family](../isa/family-control.md).
A discard or a demote instead restores the pre-quad mask, masks the active lanes
and does **not** execute `exit`.

## Stage conventions

### Vertex

Outputs go to stream SIO0: the position record at offset `0x0000` and the
varying records after it, each written as a four-channel transfer, followed by
one export that commits the total channel count.  When the vertex work is fused
into a geometry stage, the export is omitted because the geometry stage reads
the ring directly.

### Fragment

Colour goes to stream SIO1.  The slot base for the first colour target is
`channels << 7`, and each target contributes two channels for 16-bit formats,
four for 32-bit formats or one for packed integers.  A written depth value
occupies channel slot 0.  The stage ends with one export carrying the total
channel count; a fragment stage that writes nothing still exports one channel.

The fragment attribute map at state offset `0x4f0` has 32 two-byte entries
mapping the fragment stage's varying-input ordinal to the producer's output
ordinal, with a generated flag and a sprite flag.  The whole map is rewritten
per draw so that stale entries cannot survive.

### Geometry

The geometry stage reads its input window from the SIO0 ring as four-channel
reads, with the channel carrying either a component of the position record or
`4 + base * 4 + component` for a varying.  Input vertex selectors arrive packed
in `v0`.

Output vertices are emitted with the emit instructions: one emit per output
vertex, with the last one terminating the primitive.  Each emit is followed by
an ordering operation, because the next emit's payload must not overwrite the
previous one's registers.  The stream header is updated with the emitted count
before the final export.

There is no attribute descriptor table for the geometry stage: its inputs are
ring channels and its outputs are written to the SIO1 ring.

### Streamout

A streamout stage is a separate kernel recorded in slot 4 with shader type 6,
a VGPR size of 2, a VGPR initial count of 2 and a tail value of `0x20`.  Its
record capacity is passed as argument 2, and its only root is the read-write
buffer root, which the kernel uses to load the destination descriptor and a
control pair: a record count and a stride in bytes.  The kernel loops over the
records, storing each through the destination descriptor, and exits.

No established graphics path installs a streamout stage: transform feedback is
written directly by the vertex or geometry stage into its targets.  The streamout
record shape and its launch registers therefore remain UNRESOLVED in hardware.

## Tessellation

Tessellation is not supported.  There is no hull shader type, no hull record, no
tessellation-level input, and the patch primitive family is rejected before any
state is built.  The domain slot is used only for the ordinary vertex role.

## Descriptor roots and binding tables

| Root | Register | Table |
|------|----------|-------|
| constant | `s4:s5` | per-stage uniform table: 12 entries of 16 bytes; entry 0 is the ordinary uniform block, entry *n* is at offset `(n-1) * 16` |
| image and sampler | `s6:s7` | 0x40-byte slots: image at `+0x00`, sampler at `+0x30` |
| attributes | `s8:s9` | 16 entries of 16 bytes; the slot index is the vertex input ordinal, read at `slot * 16` |
| read-write buffer | `s0:s1` | storage table: 16 entries of 16 bytes, entry *n* at `n * 16`; fragment stage only |

The constant-buffer root (root 2) is written by hosts but has no launch effect,
and the LDS size and kernel config fields are always zero; a consumer must not
infer behaviour from them.

## Limits

The register files, the reserved windows, the launch-supplied registers and the
per-stage size rules are in [Registers and Allocation](registers.md).  The
remaining limits of a stage are:

| Limit | Value |
|-------|-------|
| Code size | 16-bit byte count |
| Clip distances | 8 |
| Vertex attributes | 16 |
| Uniform buffer bindings per stage | 12 |
| Storage buffer bindings | 16 (fragment stage only) |
| Fragment attribute map entries | 32 |
| Interpolation coefficients | 63 |
| Geometry varying planes | 5 |
| Render targets | 8 |
| Push constants | 256 bytes |

## UNRESOLVED

* The geometry stage's uniform-buffer table placement.
* The meaning of the extra high bit some hosts set in the geometry resource root.
* The "generated" flag of a fragment attribute entry.
* The LDS size and kernel config fields.
* The streamout record contract in hardware, and how its read-write root is
  hydrated.
* The convention for an absent resource: hosts disagree between a fully zeroed
  descriptor and a descriptor pointing at a zeroed scratch range.
