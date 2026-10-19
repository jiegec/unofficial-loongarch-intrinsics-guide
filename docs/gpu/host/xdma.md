# XDMA

The transfer pipe: linear copy and fill. It uses `STSCFG`/`SBMT` on its own
L1 ring; it does not use the GFX IB path.

## Ring and ownership

| Property | Value |
|----------|-------|
| ICMD MOP | `0x04` (`BDQ`, `DQSZ`, `UBDQ`) |
| Cursors | `0x20` write / `0x24` read, dwords |
| Completion | IRQ source `0x23` (`XDMA`) |

## Command stream

```text
SCMD32(STSCFG, 8 << 20)
element/operation
height/width
source or fill value
source address / destination address
source pitch / destination pitch
SCMD32(SBMT, 0)
```

The reviewed enum is `copy=1`, `fill=7`; element size is `4 bytes=0` or
`8 bytes=1`.

| Form | Element | Operation | Fields |
|------|--------:|----------:|--------|
| linear copy | `1` | `1` | source VA49 + UMAP, destination VA49 + UMAP, two pitches |
| linear fill | `0` | `7` | fill value32, destination VA49 + UMAP, destination pitch |

Width and height are 16-bit fields. Other operation/element combinations are
not assigned by the current LG200 producer schema.
