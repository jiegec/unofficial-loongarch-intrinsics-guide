# XDMA

XDMA is the transfer engine for linear copy and fill. It has its own ring and
does not use the graphics indirect-buffer format.

## Ring and ownership

| Property | Value |
|----------|-------|
| ICMD block | `0x04` |
| Ring cursors | write `0x20`, read `0x24` |
| Completion | IRQ source `0x23` |
| IB alignment | start 256 B, length 8 B |

## Command stream

An XDMA operation is a sequence of stream command words and payload words:

```text
SCMD32(STSCFG, 8 << 20)
configuration
dimensions
...
SCMD32(SBMT, 0)
```

`STSCFG` and `SBMT` are the same stream opcodes used by GPIPE. The
configuration word selects the operation and the element size. The current
driver emits:

| Operation | Configuration word |
|-----------|--------------------|
| Copy | `(1 << 0) OR (1 << 8)` |
| Fill | `(1 << 0) OR (7 << 8) OR (page_size << 26)` |

The page-size field is 1 for a 4 KiB kernel and 3 for a 16 KiB kernel. The
element size is encoded in the operation field by the driver; the interface
defines 4-byte and 8-byte elements.

## XDMA Copy

| Word | Content |
|-----:|---------|
| 0 | `SCMD32(STSCFG, 8 << 20)` |
| 1 | Configuration: copy. |
| 2 | `(height << 16) OR width`. |
| 3 | Source address, low 32 bits. |
| 4 | Source address, high 32 bits, OR the UMAP flag. |
| 5 | Destination address, low 32 bits. |
| 6 | Destination address, high 32 bits, OR the UMAP flag. |
| 7 | Source pitch in bytes. |
| 8 | Destination pitch in bytes. |
| 9 | `SCMD32(SBMT, 0)`. |

## XDMA Fill

| Word | Content |
|-----:|---------|
| 0 | `SCMD32(STSCFG, 8 << 20)` |
| 1 | Configuration: fill, with the page-size field. |
| 2 | `(height << 16) OR width`. |
| 3 | Fill value, low 32 bits. |
| 4 | Fill value, high 32 bits, OR the UMAP flag. |
| 5 | Destination address, low 32 bits. |
| 6 | Destination address, high 32 bits, OR the UMAP flag. |
| 7 | Zero. |
| 8 | Destination pitch in bytes. |
| 9 | `SCMD32(SBMT, 0)`. |

The UMAP flag is `0x0002_0000`. It marks an address that is already a physical
or aperture address rather than a GPU virtual address.

## Alignment and limits

The source and destination addresses and pitches MUST be aligned to the element
size. The width and height are 16-bit fields; a transfer larger than 65535
elements in one dimension is split into several packets. The size alignment of
the IB is 8 bytes; the start alignment is 256 bytes.

## Completion

An XDMA submission completes through its fence and the XDMA end-of-pipe
interrupt. A copy and a fill use the same completion path.

## Open questions

- The full operation and element-size enumeration beyond copy and fill.
- Whether the pitch is in bytes or elements for every operation.
- The behaviour of an unaligned address or pitch.
