# BPIPE

BPIPE is the inner graphics command processor. It consumes the command stream
inside a graphics indirect buffer after GPIPE accepts the state record, and it
also drives blits and clears. A BPIPE command is a 32-bit word:

```text
31              20 19              8 7               0
+-----------------+-----------------+-----------------+
|      op1        |      op0        |      command    |
+-----------------+-----------------+-----------------+
```

```text
BPIPE_CMD(cmd, op0, op1) =
    (cmd & 0xff) | ((op0 & 0xfff) << 8) | ((op1 & 0xfff) << 20)
```

## Commands

| Opcode | Name | Payload |
|-------:|------|---------|
| `0x00` | `NULL` | None. |
| `0x01` | `CFGW` | `op1` dwords follow, each a reserved word and a data word. |
| `0x02` | `CFGD` | One 64-bit configuration value. |
| `0x03` | `DRAW` | A draw-call payload. |
| `0x04` | `CINV` | Cache invalidate. |
| `0x05` | `SYNC` | Flush selector; graphics uses stage 0 and selector `0xf`. |
| `0x06` | `WAIT` | Wait at stage `op0` for an end-of-pipe message. |
| `0x07` | `SEND` | Obsolete message form. |
| `0x08` | `CTXD` | Context dump address, 64 bits. |
| `0x09` | `CTXL` | Context load address, 64 bits. |
| `0x0a` | `CTXR` | Context reset. |
| `0x0b` | `CTXS` | Context save; fields UNRESOLVED. |
| `0x0c` | `STSD` | Status dump address, 64 bits. |
| `0x0d` | `STSR` | Status reset. |
| `0x0f` | `VMID` | Update the pipe's domain; disabled for ordinary IBs. |
| `0x10` | `WR32` | Address 64 bits and 32-bit data at end of pipe. |
| `0x11` | `WR64` | Address 64 bits and 64-bit data at end of pipe. |
| `0x12` | `EVNT` | Event counter sample address, 64 bits. |
| `0x13` | `TIME` | Clock sample address, 64 bits. |
| `0x14` | `INTR` | Interrupt at end of pipe. |
| `0x15` | `MSIW` | MSI interrupt at end of pipe. |
| `0x16` | `DBAR` | Wait for writes and read back the last address. |
| `0x18` | `SEMI` | Semaphore initialize. |
| `0x19` | `SEMW` | Semaphore wait. |
| `0x1a` | `SEMR` | Semaphore read. |

## Blit sequence

A blit is a context reset, a configuration write carrying the draw-call payload,
a draw, a flush and padding to an eight-dword boundary:

```text
CTXR(0x0a)
CFGW(0x01, payload_dwords)
<payload>
DRAW(0x03, no_skip)
SYNC(0x05, stage 0, selector 0xf)
NOP padding
```

## Draw-call payload

The payload describes one fill, clear or blit. It is a packed structure, not a
shader ABI.

| Offset | Field | Meaning |
|-------:|-------|---------|
| `0x00` | `draw_op` | 0 fill, 1 clear, 2 blit. |
| `0x00` | `ras_dir` | 0 left-bottom, 1 left-top, 2 right-bottom, 3 right-top. |
| `0x00` | `rot_xy` | Rotate by 90 or 270 degrees. |
| `0x00` | `cst_fill` | Use the constant fill colour; no source. |
| `0x00` | `maskblit` | Enable the mask texture. |
| `0x00` | `tex_rot` | Rotate the source texture. |
| `0x00` | `tex_swizzle` | 12-bit source channel swizzle. |
| `0x00` | `mask_swizzle` | 12-bit mask channel swizzle. |
| `0x04` | `single_gpc` | Use a single GPC. |
| `0x04` | `camask_en` | Enable the coverage mask. |
| `0x04` | `wmask_en` | Enable the write mask. |
| `0x04` | `gpc_block_x/y` | Three-bit block selectors. |
| `0x08` | `box_x0/y0` | 16-bit box origin. |
| `0x0c` | `box_x1/y1` | 16-bit box end. |
| `0x10` | `tex_s` | Source coordinate S, signed 1.16.15. |
| `0x14` | `tex_t` | Source coordinate T. |
| `0x18` | `tex_ds` | Source delta S, signed 1.7.24. |
| `0x1c` | `tex_dt` | Source delta T. |
| after coords | source texture | Base address (40 bits used), width, height, wrap, filter, pattern, power-of-two pad, memory type, data format and pitch. |
| after source | destination surface | Base address, format, type, tiling, swizzle, width, height, depth, pitch, layers, levels and alignment. |
| after destination | blend | Enable, RGB and alpha equations, source and destination factors, colour mask. |
| after blend | logic | Independent blend, dual-source blend, logic enable and logic function. |
| after logic | blend colour | 64-bit constant colour. |
| after colour | mask texture | Coordinates, deltas and a texture record with the same shape as the source record. |
| after mask | mask blend | Mask blend enable, equations, factors and colour mask. |
| after mask blend | fill pattern | 128 dwords of fill colour. |

The source and destination records use the same format, surface-type and tiling
vocabulary as the graphics state object. The exact byte offset of each block
after the coordinate words is not established independently of the payload
builder:

> UNRESOLVED: the absolute offset and the reserved gaps between the texture,
> surface, blend, mask and fill-pattern blocks; the field order within each
> block is established, the padding is not.

## Limits

A BPIPE indirect buffer is aligned to 32 bytes at the start and 8 bytes for its
length. The payload length is carried in the `CFGW` command. A payload larger
than the buffer is a host error; the command processor does not bounds-check it.

## Open questions

- The semantics of `CTXS`, `SEMI`, `SEMW` and `SEMR`.
- Whether the mask texture uses the same coordinate format as the source.
- The meaning of the `single_gpc` and block selectors.
