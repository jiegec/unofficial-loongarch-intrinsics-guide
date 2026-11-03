# BPIPE

BPIPE is the blit pipe. It performs one rectangle operation at a time — a fill or
a blit — between an optional source texture, an optional mask texture and one
destination surface, described by a configuration-register image and started by a
draw command that carries no payload of its own.

The pipe executes no shader, and its command set carries no state record, no
Draw Info, no vertex or index fetch, no primitive assembly and no depth, stencil
or sample state: a buffer holds pipe command words, not [state
records](../graphics/state-records.md). One destination record means one colour
surface, so an operation cannot express a second render target.

## Ring

BPIPE has its own ring, its own cursors and its own end-of-pipe interrupt.

| Property | Value |
|----------|-------|
| Immediate-command block | `0x02`; operation `0x01` binds the ring base (64-bit address), operation `0x02` its size in dwords |
| Write cursor | register `0x30` |
| Read cursor | register `0x34` |
| End-of-pipe interrupt | source `0x02` |
| Alignment reported to a client | 32 B start, 8 B size |

The ring hands the pipe a buffer with a domain-select word (`0x0f`, VMID in
`[31:8]`) and a four-dword indirect-buffer packet: `0x80`, address low, address
high, and a control word carrying the dword count in `[23:0]` and the VMID in
`[31:24]`. Cursor units, frame publishing, ring sizing and buffer admission are
common to the kernel rings and are in [Rings](rings.md). The reported alignments
are declarations; the rule enforced on a submitted buffer is a 4-byte aligned
start and a nonzero length that is a multiple of 4 bytes of at most
`0x00ff_ffff` dwords.

A submission selects this engine with `ip_type` `LOONGGPU_HW_IP_BPIPE`.
Admission checks that the buffer is reachable and inside its object, not what its
words mean, so a buffer built for another pipe is not caught there.

The domain is selected on the ring, before the buffer that uses it, and the
pipe-level domain command MUST NOT appear inside a buffer. The vocabularies also
differ in meaning for the same opcode byte: `0x80` names an indirect buffer on
the ring and is not a pipe command.

## Command words

Every word in a BPIPE indirect buffer has this form:

```text
31            20 19            8 7              0
+---------------+----------------+---------------+
|      op1      |      op0       |    command    |
+---------------+----------------+---------------+

word(command, op0, op1) =
    (command & 0xff) | ((op0 & 0xfff) << 8) | ((op1 & 0xfff) << 20)
```

| Opcode | Name | Function and operands |
|-------:|------|---------------------------------------------|
| `0x00` | `NULL` | No effect. The word is 0, which is also the ring no-operation word. |
| `0x01` | `CFGW` | Configuration write. `op1` is the number of data dwords that follow; `op0` names the first configuration register and is 0 in every sequence. |
| `0x02` | `CFGD` | Configuration write of one 64-bit value. |
| `0x03` | `DRAW` | Start the operation the configuration image describes. `op0` is a skip flag, `op1` is 0. |
| `0x04` | `CINV` | Cache invalidate. |
| `0x05` | `SYNC` | Cache flush. `op0` is `0x0f` in the blit sequence. |
| `0x06` | `WAIT` | Wait at stage `op0` for an end-of-pipe message. |
| `0x07` | `SEND` | Message send; obsolete. |
| `0x08` | `CTXD` | Context dump; a 64-bit address follows. |
| `0x09` | `CTXL` | Context load, performance counters included; a 64-bit address follows. |
| `0x0a` | `CTXR` | Context reset. |
| `0x0b` | `CTXS` | Context save. |
| `0x0c` | `STSD` | Status dump; a 64-bit address follows. |
| `0x0d` | `STSR` | Status reset. |
| `0x0f` | `VMID` | Change the pipe's domain. MUST NOT appear in a buffer. |
| `0x10` | `WR32` | Write 32 bits at end of pipe. |
| `0x11` | `WR64` | Write 64 bits at end of pipe. |
| `0x12` | `EVNT` | Sample the pipe's event counter. |
| `0x13` | `TIME` | Sample the pipe's clock counter. |
| `0x14` | `INTR` | Raise an interrupt at end of pipe. |
| `0x15` | `MSIW` | Send an MSI at end of pipe. |
| `0x16` | `DBAR` | Write barrier: wait for the writes to land, then read the last address back. |
| `0x18` | `SEMI` | Semaphore initialise. |
| `0x19` | `SEMW` | Semaphore wait. |
| `0x1a` | `SEMR` | Semaphore read. |

Opcodes `0x0e` and `0x17` are unassigned. Four commands have a documented
operand: `CFGW` in `op1`, and `DRAW`, `SYNC` and `WAIT` in `op0`.
`CTXD`, `CTXL` and `STSD` are followed by a 64-bit address; the operand
layout of `WR32`, `WR64`, `EVNT` and `TIME` — including whether the write
commands append their data — and the meaning of `CTXS`, `SEMI`, `SEMW` and
`SEMR` are not established.

> UNRESOLVED: `CFGW` is documented as the pair `{null32, data32}`, while every
> emitted sequence writes one count in `op1` followed by that many consecutive
> data words. A driver MUST emit the count form; whether the pipe also accepts
> the paired form, and whether `op0` is a dword or a byte offset, is not
> established.
>
> `DRAW` is documented as `{null64}`, but no payload words follow it — the word
> behind it is the flush. Whether the pipe consumes two words there, and the flush
> survives only because its value is not read as an operand, is not established.

## A blit

One operation is a context reset, a configuration write carrying the whole image,
a draw and a flush, padded to an 8-dword boundary. A fill and a blit use this
sequence and differ only in the image. A clear has no sequence of its own either:
`cst_fill` in a fill image is what writes a constant colour over the box.

| Dword | Word | Meaning |
|------:|------|---------|
| 0 | `0x0000_000a` | `CTXR` — context reset |
| 1 | `0x0a20_0001` | `CFGW` — `op1` = 162, the number of data dwords |
| 2–163 | image | the configuration image, 162 dwords |
| 164 | `0x0000_0003` | `DRAW` — `op0` = 0, the skip flag unset |
| 165 | `0x0000_0f05` | `SYNC` — `op0` = `0x0f` |
| 166–167 | `0x0000_0000` | no-operation padding |

The padding is `(8 - (position & 7)) % 8` words measured from the start of the
buffer, so a sequence that begins at dword 0 occupies 168 dwords, 672 bytes. The
buffer's length MUST reach the flush: a length that stops at `DRAW` leaves the
writes unflushed. The reset opens every sequence, so the image is written into a
reset context rather than on top of the fields the previous operation left
behind.

## The configuration image

| Offset | Block | Fields |
|-------:|-------|--------|
| `0x00` | Draw type | `draw_op` `[1:0]`, `ras_dir` `[3:2]`, `rot_xy` `[4]`, `cst_fill` `[5]`, `maskblit` `[6]`, `tex_rot` `[7]`, `tex_swizzle` `[19:8]`, `mask_swizzle` `[31:20]` |
| `0x04` | Draw null | `single_gpc` `[0]`, `camask_en` `[1]`, `wmask_en` `[2]`, `gpc_block_x` `[5:3]`, `gpc_block_y` `[8:6]`, reserved `[31:9]` |
| `0x08` | Box origin | `box_x0` `[15:0]`, `box_y0` `[31:16]`; destination pixels |
| `0x0c` | Box end | `box_x1` `[15:0]`, `box_y1` `[31:16]`; destination pixels, the last pixel of the box |
| `0x10`, `0x14` | `tex_s`, `tex_t` | source S and T, signed 1.16.15, texels |
| `0x18`, `0x1c` | `tex_ds`, `tex_dt` | source delta per destination pixel, signed 1.7.24 |
| `0x20` | Source base | 64-bit address; only the low 40 bits are used |
| `0x28` | Source size | `width` `[15:0]`, `height` `[31:16]`; texels |
| `0x2c` | Source resource | `wrap` `[0]`, `filter` `[1]`, `pattern` `[2]`, `pow2p` `[3]`, `mtype` `[7:4]`, `dfmt` `[15:8]`, `pitch` `[31:16]` |
| `0x30` | Destination base | 64-bit address |
| `0x38` | Destination format | `format` `[7:0]`, `type` `[11:8]`, `tilem` `[15:12]`, `swizzle` `[27:16]`, `swzlrev` `[28]`, `pow2p` `[29]`, reserved `[31:30]` |
| `0x3c` | Destination size | `width` `[15:0]`, `height` `[31:16]`; pixels |
| `0x40` | Destination depth and pitch | `depth` `[15:0]`, `pitch` `[31:16]`; pixels |
| `0x44` | Destination layers and levels | `base_layer` `[11:0]`, `last_layer` `[23:12]`, `base_level` `[27:24]`, `last_level` `[31:28]` |
| `0x48` | Destination mip and alignment | `level` `[3:0]`, `align` `[7:4]`, reserved `[31:8]`; `0x4c` reserved |
| `0x50` | Blend | `blend_enable` `[0]`, `rgb_func` `[3:1]`, `rgb_src_factor` `[8:4]`, `rgb_dst_factor` `[13:9]`, `alpha_func` `[16:14]`, `alpha_src_factor` `[21:17]`, `alpha_dst_factor` `[26:22]`, `colormask` `[30:27]`, reserved `[31]` |
| `0x54` | Logic | `indep_blend_en` `[0]`, `dualsrc_blend_en` `[1]`, reserved `[2]`, `logicop_en` `[3]`, `logicop_func` `[7:4]`, reserved `[31:8]` |
| `0x58` | Blend colour | 64-bit constant, low word here, high word at `0x5c` |
| `0x60` | Mask | coordinates at `0x60`, `0x64`, `0x68`, `0x6c` in the two source-coordinate formats; a texture record at `0x70` shaped like `0x20`–`0x2c`; a blend word at `0x80` shaped like `0x50`; `0x84` reserved |
| `0x88` | Fill pattern | 128 dwords of colour |

The destination block is the colour-buffer record of the graphics state object at
the same bit positions, so one routine fills both; see [State
object](../graphics/state-object.md). The source block is not the 32-byte image
descriptor a shader reads: base 40 bits, width and height 16 bits each, data
format 8 bits.

The source rectangle has no end coordinate. It is `tex_s`, `tex_t` plus the
deltas, so the source extent is the destination extent multiplied by them. A 1:1
blit writes `1 << 24` in both deltas; the general case is source extent divided
by destination extent.

> UNRESOLVED: `swzlrev`, `single_gpc`, `camask_en`, `wmask_en`, `gpc_block_x`,
> `gpc_block_y`, the `type` values other than 0, `mtype` value 1 and the
> `level`/`align` pair for a non-zero mip have no established meaning, so a
> driver has no basis for a non-zero value. Whether a fill that sets `pattern`
> reads the block at `0x88` is also open: the block's register names run to
> `0x25c`, ten dwords short of the array.

### Field values

`draw_op` is 0 fill and 2 blit. Value 1 is called `BE_CLEAR` in the comment on
the field, but no enumerator defines it and nothing writes it, so what it does is
UNRESOLVED; value 3 is undefined. A rectangle clear has no operation of its own:
`cst_fill` in a fill image takes the colour from the blend constant and reads no
source. `ras_dir` is 0 left-bottom, 1 left-top, 2 right-bottom, 3 right-top.
`tilem` and `mtype` are 0 linear, 2 tiled 4, 3 tiled 8, and 1 is not established.
`wrap` is 0 clamp-to-edge or 1 repeat, `filter` is 0 nearest or 1 linear,
`pow2p` applies a power-of-two pad, and `pattern` makes the source a fill pattern
instead of an image.

Each 3-bit channel selector names a component: 0 X, 1 Y, 2 Z, 3 W, 4 constant 0,
5 constant 1. A 12-bit swizzle packs four of them, R in `[2:0]`, G in `[5:3]`,
B in `[8:6]`, A in `[11:9]`; the identity is `0x688`, and the 32-bit path
takes R from Z, G from Y, B from X and A from W (`0x60a`).

| `format` | Destination | `format` | Destination |
|------:|-------------|------:|-------------|
| 0 | 8-8-8-8 | 34 | 5-6-5 |
| 20 | 8 | 44 | 10-10-10-2 |
| 30 | 2-10-10-10 | 45 | 5-5-5-1 |
| 33 | 1-5-5-5 | — | — |

| `dfmt` | Source | `dfmt` | Source |
|------:|-------------|------:|-------------|
| `0x01` | 8 | `0x0a` | 8-8-8-8 |
| `0x02` | 16 | `0x0c` | 16-16-16-16 |
| `0x05` | 16-16 | `0x10` | 5-6-5 |
| `0x06` | 10-11-11 | `0x11` | 1-5-5-5 |
| `0x07` | 11-11-10 | `0x12` | 5-5-5-1 |
| `0x08` | 10-10-10-2 | `0x13` | 4-4-4-4 |
| `0x09` | 2-10-10-10 | — | — |

Destination values 0, 20 and 44 agree with the graphics colour formats RGBA8, R8
and RGB10A2; 30, 33, 34 and 45 are packed formats with no entry there. Source
values `0x01`, `0x02`, `0x05`, `0x06`, `0x09`, `0x0a` and `0x0c` agree with the
graphics image data formats; `0x10`–`0x13` do not.

The blend word has the layout and the enumerations of the graphics per-target
blend word ([Blending](../graphics/blending.md)): `rgb_func` and `alpha_func`
are 0 add, 1 subtract, 2 reverse subtract, 3 minimum, 4 maximum; the factors run
from 1 one and 2 source colour to 10 source 1 alpha, then 17 zero and 18–21 and
23–26 for the one-minus forms; `colormask` bit 0 is R through bit 3 A, so `0xf`
writes all four. A constant-colour fill enables blending, takes `CONSTANT_COLOR`
for RGB and `CONSTANT_ALPHA` for alpha as the source factors, zero as both
destination factors, and puts the colour in `blend_color`.

## Limits

| Limit | Value |
|-------|-------|
| Configuration image | 162 dwords, 648 bytes |
| `CFGW` data count | 12 bits; at most 4095 dwords |
| Indirect buffer length | nonzero multiple of 4 bytes, at most `0x00ff_ffff` dwords |
| Box coordinate | 16 bits, 0–65535 destination pixels |
| Source coordinate | 16-bit integer part; 0–65535 texels expressible |
| Source delta | signed 1.7.24; below 128 in magnitude |
| Surface width, height, pitch, depth | 16 bits each |
| Layers and mip levels | base and last, 12 bits and 4 bits |
| Fill pattern | 128 dwords |

The image length is the only length the pipe receives, and nothing in the stream
bounds it against the buffer: a `CFGW` count that runs past the end of the buffer
is a host error, and what the pipe fetches there is not established. The same
holds for a box that leaves its surface — the pipe is told the surface size, but
whether it clips or writes on is not established.

A scale factor of 128:1 or more has no 1.7.24 encoding. The failure value,
`0xffff_ffff`, is itself a valid negative delta, so a driver MUST keep source and
destination extents within 127:1 and MUST NOT rely on the field to report the
overflow.

## Open questions

- Whether `draw_op` 1 names a real operation: only a comment calls it
  `BE_CLEAR`, no enumerator defines it, nothing writes it, and the constant-colour
  clear is reachable as a fill with `cst_fill` set.
- What `CTXR` clears, which decides whether a later operation may omit fields the
  reset would otherwise have cleared.
- Whether `rot_xy` and `tex_rot` select 90 or 270 degrees; one bit names both.
- Whether a blit may read and write the same surface.
- Whether the 8-dword end padding is required, or only a convention of the writer.
