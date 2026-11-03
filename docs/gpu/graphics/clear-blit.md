# Clear and Blit

A clear writes one value over a rectangle; a blit copies a rectangle, possibly
scaled and possibly with channels reordered. Neither is an opcode of its own on
LG200: a clear is a draw or a host write, and a blit is either a draw that samples
the source or a fixed-function draw call on the BPIPE engine. This chapter fixes
which shape goes to which engine, the box convention the engine uses, the two
format vocabularies it needs, and the ordering a host must add. The command words
and the layout of the whole configuration image are in
[BPIPE](../host/bpipe.md); the surface record shared with the graphics state is
in [State object](state-object.md).

## Engine selection

| Operation | Engine | How |
|-----------|--------|-----|
| Whole-surface colour clear, all channels | graphics pipe | full-viewport quad, constant-colour fragment shader |
| Scissored clear | graphics pipe | the same quad with the scissor test enabled |
| Depth or stencil clear of one layer | graphics pipe | the same quad; depth is the quad's Z |
| Client blit: copy, scale, format change | graphics pipe | full-viewport draw that samples the source |
| Partial channel mask, pure-integer colour, layered depth, texture level | host CPU | per-pixel write in the surface layout |
| Blit shape the sampling draw cannot serve | host CPU | element copy in the surface layout |
| Display framebuffer copy into the scanout buffer | BPIPE | draw call, `draw_op` = 2 |
| Constant fill, flat or patterned | BPIPE | draw call, `draw_op` = 0, `cst_fill` = 1 |

The BPIPE ring is kernel-owned: the user-queue interface accepts only
`LOONGGPU_HW_IP_COMPUTE`, so a client never submits a blit. On this generation
the engine is driven by the display path, which copies a framebuffer into the
scanout buffer and may scale it in the same call. A client clear or blit therefore
reaches the graphics pipe or the host CPU, and BPIPE is reached only through the
kernel.

## The draw call

A BPIPE operation is one full configuration image, not a state delta: context
reset, one configuration-write carrying the whole image, the draw, a cache flush,
then padding of the command buffer to an eight-dword boundary. The reset makes the
image self-contained: every field is written on every call, so none may be assumed
to hold a value from an earlier call.

| Offset | Field (bits) | Meaning |
|--------|--------------|---------|
| `0x00` | `draw_op` `[1:0]` | 0 fill, 2 blit |
| `0x00` | `ras_dir` `[3:2]`, `rot_xy` `[4]`, `cst_fill` `[5]` | raster corner, 90 or 270 degree rotation, constant fill |
| `0x00` | `maskblit` `[6]`, `tex_rot` `[7]`, `tex_swizzle` `[19:8]` | masked blit, mask rotation, source channel select |
| `0x08`, `0x0c` | `box_x0`, `box_y0`, `box_x1`, `box_y1` | destination box, inclusive end |
| `0x10`-`0x1c` | `tex_s`, `tex_t`, `tex_ds`, `tex_dt` | source origin and per-pixel step |
| `0x20`-`0x2c`, `0x30`-`0x4c` | source and destination records | see below |
| `0x50`, `0x58` | blend word, 64-bit constant colour | constant-fill equation |
| `0x88` | fill pattern, 128 dwords | patterned fill |

Two operation codes are defined: 0 for a fill and 2 for a blit. The
field's annotation names a third operation, a clear, between them, but no symbol
gives it a value and no path programs it. A constant clear is a fill with
`cst_fill` set and the colour in the constant. Whether the value between fill and
blit is a distinct clear, and what it does that a constant fill does not, is
UNRESOLVED.

## Box units and clipping

The host-facing box is a pair of corners with an exclusive far edge. The packet
stores the far edge minus one:

| Host corner | Register | Value |
|-------------|----------|-------|
| `x1`, `y1` | `box_x0`, `box_y0` | copied unchanged |
| `x2`, `y2` | `box_x1`, `box_y1` | `x2 - 1`, `y2 - 1` |

`box_x1` is therefore the **inclusive** last destination pixel. A host that
writes an exclusive end into it covers one extra column and one extra row. Each
field is 16-bit unsigned, so a box whose last column or row exceeds 65535 cannot
be expressed. The box is measured in destination pixels; there is no second box
for the source.

Clipping is the host's responsibility. Nothing in the image compares the box
against the destination record's width and height, so a host MUST keep the box
inside the destination surface. What a box that leaves the surface does — clip,
wrap, or write past it — is UNRESOLVED.

## Source coordinates and scaling

| Field | Encoding | Value for 1.0 | Programmed with |
|-------|----------|---------------|-----------------|
| `tex_s`, `tex_t` | signed 1.16.15 | `1 << 15` | source origin, `x1 << 15` and `y1 << 15` |
| `tex_ds`, `tex_dt` | signed 1.7.24 | `1 << 24` | source texels per destination pixel, `sw / dw` and `sh / dh` |

A 1:1 copy programs `1 << 24` in both deltas. Any other ratio is the source
extent divided by the destination extent, rounded to nearest. A ratio below one
magnifies, above one minifies, and both use the same two fields.

The 1.7.24 range is plus or minus 128, so a scale of 128:1 or more has no
encoding. The value a failed conversion leaves behind is a legal small negative
step, so the field cannot report the overflow: a host MUST bound the extents
itself.

The sample position is computed from the origin and the step, with no clip
against the source box. Destination column `k` samples at `x1 + k * sw / dw`, so
the last column samples one step short of the exclusive far edge, at
`x2 - sw / dw`. A linear filter then reads up to one texel on either side of that
point, which can leave the box; at the surface edge the wrap mode decides what is
read. Only the source record's width, height and wrap mode bound the fetch.

## Surface records

The destination is a 32-byte record at the same bit positions as the graphics
colour-buffer record: base address, format, surface type, tiling mode, channel
swizzle, swizzle reverse, power-of-two pad, width, height, depth, pitch, base and
last layer, base and last mip, level and alignment. Width, height and pitch are
counts of pixels, not bytes.

The source is a 16-byte record of its own, not the 32-byte image descriptor a
shader reads:

| Offset | Field (bits) | Meaning |
|--------|--------------|---------|
| `0x00` | base address `[63:0]` | only the low 40 bits are used |
| `0x08` | `width` `[15:0]`, `height` `[31:16]` | texels |
| `0x0c` | `wrap` `[0]`, `filter` `[1]`, `pattern` `[2]`, `pow2p` `[3]` | edge mode, filter, pattern fill, power-of-two pad |
| `0x0c` | `mtype` `[7:4]`, `dfmt` `[15:8]`, `pitch` `[31:16]` | tiling, source format, pitch in texels |

`wrap` is 0 clamp-to-edge or 1 repeat; `filter` is 0 nearest or 1 linear. Both
pitches count elements, not bytes, so a host converts a byte stride before
programming either.

## Two format vocabularies

The two records number formats differently, and each side resembles a different
graphics enumeration rather than sharing one:

| Layout | Source `dfmt` | Graphics image format | Destination `format` | Graphics colour format |
|--------|---------------|-----------------------|-----------------------|------------------------|
| 8-8-8-8 | `0x0a` | RGBA8 = 10 | 0 | RGBA8 = 0 |
| 8 | `0x01` | R8 = 1 | 20 | R8 = 20 |
| 5-6-5 | `0x10` | none | 34 | none |
| 1-5-5-5 | `0x11` | none | 33 | none |
| 5-5-5-1 | `0x12` | none | 45 | none |
| 4-4-4-4 | `0x13` | none | none | none |
| 10-10-10-2 | `0x08` | none | 44 | RGB10A2 = 44 |

The same 8-8-8-8 surface is `0x0a` on the source side and 0 on the destination
side, so one format number cannot serve both fields. Each record also defines
codes that the graphics enumerations lack — the packed 16-bit codes on the source
side, 30, 33, 34 and 45 on the destination side — so a host MUST map its own
format to each field separately instead of reusing one shared code. Neither table
has a depth or stencil code, although the graphics image data format defines one
(21): the defined vocabulary covers colour surfaces only, and whether the pipe
accepts a depth code is UNRESOLVED.

Tiling has the same split. The host tiling code is 1 linear, 2 tiled 4, 3 tiled 8;
the record takes 0 linear, 2 tiled 4, 3 tiled 8. Copying the host code into the
record programs 1 for a linear surface, a value no path programs and whose meaning
is not established.

## Channel selection, not conversion

Neither record carries a number format, so a blit cannot convert. It fetches a
texel, selects four channels and writes them.

| Select | Channel |
|--------|---------|
| 0-3 | X, Y, Z, W |
| 4 | constant 0 |
| 5 | constant 1 |

Each swizzle packs four 3-bit selects, R in `[2:0]` through A in `[11:9]`; the
identity is `0x688`. Three fields hold them: the destination record's `swizzle`,
`tex_swizzle` for the source and `mask_swizzle` for the mask texture. A
framebuffer copy programs the first two identically: R from Z, G from Y, B from X
and A from W (`0x60a`), so that a little-endian `0xAARRGGBB` word lands as R, G,
B, A bytes. A 16-bit destination takes R, G and B from X, Y and Z and alpha from
constant 1; an 8-bit destination takes R, G and B from constant 0 and alpha from
X.

A copy leaves blending disabled, so the selected channels replace the destination,
and the four-bit channel mask in the blend word decides which of them are stored.
A fill enables blending with the constant colour as source factor and zero as
destination factor, so the destination receives the constant.

What each destination channel receives is therefore a host choice, not a format
rule. Three consequences follow:

- A copy between surfaces with the same channel widths but different number
  formats moves bit patterns: no unorm, float or integer conversion happens.
- No sRGB code exists in either vocabulary, so a blit cannot apply or remove the
  sRGB transform. Whether the 8-8-8-8 code is read as sRGB when the destination
  surface is sRGB is UNRESOLVED.
- A channel taken from a constant does not come from the source: a 16-bit
  destination receives alpha 1 whatever the source alpha was, and an 8-bit
  destination receives black in R, G and B.

## Clear as a draw

A clear on the graphics pipe is a draw: one quad covering the viewport, a fragment
shader that writes the constant colour, depth from the quad's interpolated Z,
stencil from the constant stencil state. Three rules follow from the hardware:

- The fragment export type must match the attachment format. A float-exporting
  clear bound to a pure-integer colour attachment causes the whole draw to be
  rejected, so no attachment of that framebuffer is cleared — not only the
  mismatched one.
- A fragment that writes no colour still needs an export. Without a null fragment
  export the hardware discards the fragment before the depth and stencil update,
  and a depth-only clear writes nothing.
- A scissored clear must run with the scissor test enabled for the duration of the
  draw, or the quad clears the whole surface.

The shapes the quad cannot serve are written by the CPU instead. A CPU-written
clear must use the surface's own layout, tiling included, or the next reader sees
the wrong pixels ([Formats and layout](formats.md)).

## Synchronisation

The draw call is queued on the BPIPE ring; completion is that ring's end-of-pipe
interrupt, which signals the submission's fence.

- A host MUST wait for that fence before reading, reusing or freeing either
  surface. A fence error or a timeout says the copy did not complete; it does not
  say the engine stopped writing, so a failed blit MUST NOT return its buffers to
  the allocator.
- The packet orders nothing against another pipe. The command vocabulary has wait
  and semaphore commands, but a blit uses none of them, so a host MUST order a
  blit against a draw, or two overlapping blits against each other, with a fence
  or a submission dependency.
- Before the CPU writes a surface the GPU may still touch, the queued work that
  references its buffer object MUST be submitted and drained and the in-flight
  submissions waited on. If that cannot be done, the write MUST be refused: a
  queued GPU clear otherwise lands after the host bytes and erases them.

## Limits

| Limit | Value |
|-------|-------|
| Box coordinate | 16 bits, 0-65535 destination pixels |
| Surface width, height, pitch | 16 bits each |
| Surface depth, layers, mip levels | 16 bits, 12 bits, 4 bits |
| Source width, height, pitch | 16 bits each |
| Source origin | signed 1.16.15 |
| Source step | signed 1.7.24, magnitude below 128 |
| Fill constant | 64 bits |
| Fill pattern | 128 dwords |
| Channel selects | four 3-bit fields per surface record |

## Open questions

- `draw_op` value 1: named as a clear in one annotation, defined and programmed
  nowhere.
- A box that leaves the destination surface: nothing in the image bounds it.
- `ras_dir`: the field annotation calls 0 left-bottom, while the framebuffer copy
  sets 0 and names it top to bottom. The vertical scan direction is UNRESOLVED, so
  a flipped copy cannot be planned from this field.
- Whether `rot_xy` and `tex_rot` select 90 or 270 degrees; one bit names both.
- Whether a 1:1 copy is bit-exact under linear filtering: where the sample point
  sits inside a destination pixel is not established.
- The masked blit: a second texture record, its coordinates and its own blend word
  exist, and the composition rule is not established.
- The patterned fill: `pattern` selects it and the image carries 128 dwords of
  pattern colour; the pattern's geometry is not established.
- Whether a blit may read and write the same surface.
- Whether a blit can address one layer or one mip: layer, level and surface-type
  fields exist, and nothing programs them.
- What the cache flush at the end of the packet covers, and whether it orders the
  destination against another pipe.
