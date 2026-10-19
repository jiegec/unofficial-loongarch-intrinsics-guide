# BPIPE

The inner graphics command stream. BPIPE consumes the commands inside a GFX IB
after the full-state object is accepted, and it also drives blits.

## Command word

```text
BPIPE_CMD(cmd, op0, op1) = (cmd & 0xff) |
                           ((op0 & 0xfff) << 8) |
                           ((op1 & 0xfff) << 20)
```

## Commands

| Command | Opcode | Payload |
|---------|-------:|---------|
| `NULL` | `0x00` | none |
| `CFGW` | `0x01` | `op1` dword count, followed by `{reserved32, data32}` words |
| `CFGD` | `0x02` | one 64-bit configuration value |
| `DRAW` | `0x03` | DTG command; producer emits null 64-bit payload |
| `CINV` | `0x04` | cache invalidate |
| `SYNC` | `0x05` | flush selector; graphics uses stage/selector `0/0xf` |
| `WAIT` | `0x06` | wait at stage `op0` for an EOP message |
| `SEND` | `0x07` | obsolete message form |
| `CTXD`/`CTXL` | `0x08/09` | context dump/load address64 |
| `CTXR`/`CTXS` | `0x0a/0b` | context reset/save; `CTXS` unresolved |
| `STSD`/`STSR` | `0x0c/0d` | status dump/reset |
| `VMID` | `0x0f` | pipe domain update; disabled for ordinary IBs |
| `WR32`/`WR64` | `0x10/11` | address64 and data at EOP |
| `EVNT`/`TIME` | `0x12/13` | event/clock sample address64 |
| `INTR`/`MSIW` | `0x14/15` | interrupt/MSI at EOP |
| `DBAR` | `0x16` | wait for writes and read back the last address |
| `SEMI`/`SEMW`/`SEMR` | `0x18..1a` | semaphore forms; payload unresolved |

The normal BPIPE blit sequence:

```text
CTXR(0x0a)
CFGW(0x01, sizeof(drawcall) / 4)
<drawcall payload>
DRAW(0x03, no_skip=0)
SYNC(0x05, stage=0, selector=0xf)
NOP padding to an eight-dword boundary
```

## BPIPE drawcall payload

The `CFGW` payload is a packed state object, not a shader ABI. Its leading
fields:

| Offset | Field | Meaning |
-------:|-------|---------|
| `0x00` | `draw_op` | `0=fill`, `1=clear` producer form, `2=blit` |
| `0x00` | `ras_dir` | `0=left-bottom`, `1=left-top`, `2=right-bottom`, `3=right-top` |
| `0x00` | `rot_xy/cst_fill/maskblit/tex_rot` | rotate/use constant/enable mask |
| `0x00` | `tex_swizzle/mask_swizzle` | 12-bit component selectors |
| `0x04` | `single_gpc/camask_en/wmask_en` | mask and partition controls |
| `0x04` | `gpc_block_x/y` | three-bit block selectors |
| `0x08/0x0c` | `box_x0/y0`, `box_x1/y1` | 16-bit rectangle coordinates |
| `0x10..0x1c` | `tex_s/t/ds/dt` | fixed-point texture coordinates/deltas |
| `0x20` | source `btex` | base64, width/height, wrap/filter, format, pitch |
| `0x40` | destination `cbuf` | base64, format/type/tile/swizzle, dimensions, levels |
| `0x60` | blend state | enable, equation/factors, channel mask |
| `0x64` | logic state | independent/dual-source/logic-op and function |
| `0x68` | blend color | four 32-bit color words |
| `0x70..0x7c` | mask coordinates | fixed-point coordinates/deltas |
| `0x80` | mask `btex` | same texture descriptor shape |
| `0x2a0` | fill pattern | 128 dwords of fill color data |

The `btex` and `cbuf` fields use the same format, surface type, and tiling
vocabulary as the graphical schema. Values not present in that schema remain
unassigned.
