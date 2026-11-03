# Depth and Stencil

The output merger resolves a fragment's depth and stencil values against the
attached surfaces before the colour write. Three records carry that test: the
16-byte Depth Control block, a 4-byte record per stencil face, and the 32-byte
depth buffer record that names the surface. Offsets are into the 512-dword state
object; add 12 for the packet byte offset ([State object](state-object.md)).

## Depth Control

Depth Control is 16 bytes at `0x324`.

| Bits | Field | Meaning |
|------|-------|---------|
| `[31:0]` | Offset units | IEEE single precision, polygon offset per depth unit. |
| `[63:32]` | Offset factor | IEEE single precision, polygon offset per unit of depth slope. |
| `[95:64]` | Offset clamp | IEEE single precision, magnitude the summed offset is limited to. |
| `[96]` | Depth enable | Run the depth test. |
| `[97]` | Depth write | Write the depth of a surviving fragment. |
| `[98]` | Clamp near | Clamp depth to the near plane instead of clipping the fragment. |
| `[99]` | Clamp far | Clamp depth to the far plane instead of clipping the fragment. |
| `[102:100]` | Depth function | One of the eight [compare functions](#compare-functions). |
| `[111:103]` | Reserved | No field. Write zero. |
| `[119:112]` | Front stencil reference | Reference value for the front face. |
| `[127:120]` | Back stencil reference | Reference value for the back face. |

The three offset fields hold raw IEEE single-precision patterns, not fixed
point. One offset unit is `2^-25` of the normalized depth range, so an `n`-bit
fixed-point API unit is written as `unit * 2^(25-n)`: doubled for 24-bit depth,
times 512 for 16-bit. The factor is written unchanged and the clamp bounds the
sum. The offset is applied to the fragment's depth before the comparison.

Depth enable clear disables the test, and every fragment then passes that stage.
Depth write is meaningful only with the test enabled, so a command stream SHOULD
clear the two together; whether the unit writes depth with the enable clear is
UNRESOLVED.

Clamp near and Clamp far are enables, not bounds. Set, a fragment outside that
plane has its depth clamped to the plane instead of being discarded, which is how
a disabled clip plane or a depth-clamp request is expressed. The planes belong to
the viewport: Z scale and Z offset place the fragment's depth, and Depth Min and
Depth Max bound it ([Rasterization](rasterization.md#viewport)). Whether the
rasterizer clamps to those bounds or only compares against them is UNRESOLVED.

The attachment is named by a second record: 32 bytes at `0x470` hold the base
address, hardware format, surface type, extent, pitch, layer range and mip range
([State object](state-object.md#depth-buffer)). A draw names a layer range and
one mip level, and the record does not carry the sample count: surface type 8
covers both 2x and 4x
([Multisampling](multisampling.md#depth-and-stencil-layout)). A draw with no
depth attachment MUST still write the record, zeroed, because the block persists
and a stale base address points the depth unit at another draw's surface.

## Stencil face record

Two records of 4 bytes, front at `0x334` and back at `0x338`. The layout is
identical; only the face differs.

| Bits | Field | Meaning |
|------|-------|---------|
| `[0]` | Enable | Run the stencil test for this face. |
| `[3:1]` | Function | Compare function over the reference and the stored value. |
| `[7:4]` | Fail operation | Applied when the stencil test fails. |
| `[11:8]` | Depth pass operation | Applied when the stencil test and the depth test both pass. |
| `[15:12]` | Depth fail operation | Applied when the stencil test passes and the depth test fails. |
| `[23:16]` | Value mask | Bits of the reference and of the stored value that the comparison sees. |
| `[31:24]` | Write mask | Bits of the stored value an operation may change. |

The reference is not in the face record: it is one byte in Depth Control, front
at `[119:112]` and back at `[127:120]`. Stencil is eight bits wide whatever the
depth format, so the stored value, both masks and both references are 8-bit.

Facing selects the record. The rasterizer derives the facing from the projected
winding of the primitive against the Front CCW convention, bit 8 of the
pipe-enable word ([State object](state-object.md#pipe-enables)); the winding rule
is in [Rasterization](rasterization.md).

The block has no single-sided mode, so a one-sided API state MUST be programmed
as two identical records: copy the front record, enable bit included, into the
back. A back record left zeroed would let a back-facing fragment pass a stencil
test the front face would have failed; a record whose Enable is clear SHOULD be
all zero, because a stale operation field is a latent write.

## Compare functions

A 3-bit code in Depth Control `[102:100]` and in `[3:1]` of each face
record. The same eight codes occupy `[14:12]` of the sampler descriptor, which
is how a sampler resolves a shadow test
([Texture and sampling](texture.md#sampler-descriptor)).

| Value | Function | Passes when |
|------:|----------|-------------|
| 0 | Never | Never. |
| 1 | Less | The incoming value is below the stored value. |
| 2 | Equal | The two values are equal. |
| 3 | Less or equal | The incoming value is below or equal. |
| 4 | Greater | The incoming value is above the stored value. |
| 5 | Not equal | The two values differ. |
| 6 | Greater or equal | The incoming value is above or equal. |
| 7 | Always | Always. |

For depth the incoming value is the fragment's depth and the stored value is the
one in the attachment; for stencil the incoming value is the face's reference and
the stored value is the surface byte. The order matches the API comparison
functions, so a driver passes its API function through unchanged. Values above 7
do not fit the fields and MUST NOT be written.

## Stencil operations

A 4-bit code in each of the three operation fields. The field's position selects
when it applies: Fail operation on a failed stencil test, Depth fail operation
when the stencil test passes and the depth test fails, Depth pass operation when
both pass.

| Value | Operation | Effect on the stored value |
|------:|-----------|----------------------------|
| 0 | Keep | Unchanged. |
| 1 | Zero | Set to 0. |
| 2 | Replace | Set to the face's reference. |
| 3 | Increment clamp | Incremented, saturating instead of wrapping. |
| 4 | Decrement clamp | Decremented, saturating instead of wrapping. |
| 5 | Increment wrap | Incremented, wrapping at the top of the range. |
| 6 | Decrement wrap | Decremented, wrapping at the bottom of the range. |
| 7 | Invert | Bitwise complement. |

Every result is written through Write mask, so bits clear in the mask keep their
stored value even for Zero and Replace. Values 8 to 15 fit the field but name no
operation, and a command stream MUST NOT write them.

## Rules

1. Write every field on every draw. The block persists and a batched record
   carries only the dwords that changed, so an omitted field keeps the previous
   draw's value. Disabled state MUST be written as its disabled value: a zeroed
   face record, a zeroed depth buffer record, Depth Write clear.
2. Depth is written only for a fragment that passes the test, and only with
   Depth write set. A draw that cannot pass the test SHOULD NOT depend on the
   write bit to suppress a commit; see rule 5.
3. The tests run before the fragment shader only when Early Z is set, bit 6 of
   the pipe-enable word. Clear, they run after the shader on the depth it
   produced. A shader that declares early fragment tests MUST have the bit set.
4. A stencil test needs a packed surface, format 39 or 40; a depth test needs a
   bound depth surface. A draw that enables either without the matching surface
   MUST be rejected before the record is built.
5. Replace commits the whole element: the unit writes the complete depth-stencil
   element, taking the depth field from the fragment — a stale zero where the
   depth test never ran — and drops the stencil bits outside Write mask. A
   command stream MUST therefore neutralise every operation the draw cannot
   reach: Depth fail operation to Keep when the depth test cannot fail, Depth
   pass operation to Keep when it cannot pass, Fail operation to Keep for a face
   whose function is Always, both to Keep for a face whose function is Never.
   When both enabled faces are Never, Depth write and Depth function MUST be
   neutralised too.
6. Format 39 holds 24-bit unsigned depth in `[31:8]` and stencil in `[7:0]`;
   format 40 holds IEEE single-precision depth in `[31:0]`, stencil in
   `[39:32]` and an unused X24 in `[63:40]`.
7. A clear of one aspect MUST mask the other: a depth-only clear of format 39
   keeps `[7:0]` (mask `0xffffff00`) and a stencil-only clear keeps `[31:8]`
   (mask `0x000000ff`); on format 40 the masks are `[31:0]` and `[39:32]`.
8. Formats 37 and 38 are depth only, unsigned 24-bit and IEEE single-precision.
   No 16-bit depth format exists: a 16-bit API depth buffer is stored in the
   format-38 layout, and a format-37 descriptor over that storage receives no
   depth writes ([Formats and layout](formats.md#depth-formats)).
9. Hi Z, bit 7 of the pipe-enable word, selects a hierarchical depth test whose
   acceptance rules are UNRESOLVED; keep it clear for a deterministic result.

## Open questions

- Whether Depth write takes effect with Depth enable clear. Until then, clear
  Depth write whenever the test is disabled.
- Where the clamp forms of increment and decrement saturate, and what the wrap
  forms roll over to.
- Whether Clamp near and Clamp far apply to a floating-point depth format, and
  how a NaN depth compares.
- Whether formats other than the two packed ones may back a stencil test. A
  stencil test needs a packed surface, but whether the unit can test the stencil
  aspect of an element whose depth aspect no draw writes is not established.
- Whether the pipe-enable Stencil Write bit, 27, gates stencil updates. No known
  command stream sets it while updating stencil, so a driver MUST NOT set it on
  the assumption that it enables writes.
