# XDMA

XDMA moves a rectangular run of memory, fills one, and updates page tables, with no
shader and no render target. Its buffers carry ten-dword state packets rather than
graphics records, and its stream has no address-space select. This chapter fixes the
packet layouts, the geometry and alignment a caller must satisfy, and what a fence
proves; ring mechanics are shared and live in [Rings](rings.md).

## Ring

| Property | Value |
|----------|-------|
| Write cursor | register `0x20`, a dword index |
| Read cursor | a writeback word; `0x24` is written once at bind, never read |
| Completion | writeback word; interrupt source id `0x23` |
| Frame budget | 256 dwords, padded to 16 dwords on publication |

There is one instance. Its queue is bound by the immediate command of block `0x04`:
`BDQ` takes the base address, `DQSZ` the size in dwords ([MMIO](mmio.md)). Nothing else
is programmed to start it, and no unbind is issued when a ring is taken out of service,
so that state is not proof the engine drained.

## The buffer packet

A submission is one or more indirect buffers, and each is named by four ring dwords:
the whole of a DMA frame in front of its fence, with no VM-select word ahead of it —
unlike GPIPE and BPIPE.

| Dword | Bits | Content |
|------:|------|---------|
| 0 | `[7:0]`, `[31:8]` | `0x80` — buffer opcode; config `0` |
| 1 | `[31:0]` | buffer address `[31:0]` |
| 2 | `[31:0]` | buffer address `[63:32]` |
| 3 | `[23:0]`, `[31:24]` | length in dwords; VMID |

The start MUST be 4-byte aligned and the length a nonzero multiple of 4 bytes, at most
`0x00ff_ffff` dwords. This engine reports a 256-byte start and an 8-byte length
alignment, stricter than the rule the submission check applies
([Rings](rings.md#ib-alignment)). A kernel buffer is padded to eight dwords with zero,
so its length is a multiple of 32 bytes; a client buffer is not padded, and its declared
count is the one the ring publishes. Each packet carries its own state config and its
own Submit word, so packets are self-contained and run in stream order.

## XDMA Copy

| Dword | Bits | Content |
|------:|------|---------|
| 0 | `[31:0]` | `0x0080_0001` — state config, eight payload dwords |
| 1 | `[7:0]`, `[15:8]` | element size; operation `1` (copy) |
| 2 | `[15:0]`, `[31:16]` | width in elements; height in lines |
| 3 | `[31:0]` | source address `[31:0]` |
| 4 | `[16:0]`, `[17]` | source address `[48:32]`; UMAP |
| 5 | `[31:0]` | destination address `[31:0]` |
| 6 | `[16:0]`, `[17]` | destination address `[48:32]`; UMAP |
| 7 | `[31:0]` | source pitch, bytes per line |
| 8 | `[31:0]` | destination pitch, bytes per line |
| 9 | `[31:0]` | `0x0000_0003` — submit |

The packet walks height lines of width elements. Element size `1` is eight bytes and
`0` four, and the packet definition fixes it at `1`. Both pitches count bytes per line,
separately per side, so a sub-rectangle of a larger surface is gathered by giving the
destination a pitch equal to its row length.

UMAP is bit 49 of an address slot — bit 17 of dword 4 and of dword 6. It marks an
address that is already a device address, which the engine must not translate; the
kernel sets it exactly when the address lies inside the VRAM aperture. Address bits
63:49 MUST be zero.

## XDMA Fill

A fill is the copy packet with the source side replaced by a value. Dwords 0, 2, 5, 6, 8
and 9 are as in [XDMA Copy](#xdma-copy); the rest are:

| Dword | Bits | Content |
|------:|------|---------|
| 1 | `[7:0]`, `[15:8]`, `[27:26]` | element size; operation `7` (fill); page size |
| 3 | `[31:0]` | value `[31:0]` |
| 4 | `[31:0]` | value `[63:32]`, zero for a four-byte fill |
| 7 | `[31:0]` | zero |

Two forms are emitted. A memory fill sets element size `0` and writes the 32-bit value
from dword 3 into every four-byte element, with dword 4 zero. A page-table fill sets
element size `1`, so the value is the 64-bit pair in dwords 3 and 4 — a page-table entry
with its flags — and names the page size in bits `[27:26]`, `1` for four KiB and `3` for
sixteen KiB. The packet definition covers only the first form: it fixes the element size
at `0` and dword 4 at zero, so the page-table form is outside it. Copy and fill are the
only operation values the format defines.

> UNRESOLVED: whether a page-table fill stores the same entry in every element or steps it
> by the page size. A page table needs the step, and the interface's own check reads
> successive entries one page apart, but no field says the value is incremented. The same
> path also sets UMAP, which is bit 49 of this 64-bit value rather than of an address.

## Splitting and limits

| Bound | Value |
|-------|-------|
| Width, height | `0xffff` elements per line, `0xffff` lines |
| One chunk, copy or fill | `0x3f_ffc0` bytes, a host maximum |
| One buffer | `0x00ff_ffff` dwords |
| Element size | `0` four bytes, `1` eight bytes |

A run longer than `0xffff` elements becomes a shorter line and more lines. The kernel
halves the width and doubles the height until it fits, so a caller MUST make the element
count divisible by the number of halvings or the last element is silently left behind.
`0x3f_ffc0` bytes divides exactly — three halvings at eight-byte elements, four at four
— and a chunk of `0x3f_ffff` bytes would drop 63.

Both addresses and both pitches are multiples of the element size in every shape the
interface builds, and a pitch is at least its row length. The size follows the shape:
eight bytes when the row length, both addresses and both pitches are all multiples of
eight, four when all are multiples of four, and otherwise the shape is not expressible.

## Completion

| Dword | Content |
|------:|---------|
| 0 | `0x10` — writeback 32 |
| 1–2 | address |
| 3 | sequence |
| 4 | `0x14` — interrupt |

Completion is the writeback, not the interrupt. The engine stores a 32-bit sequence in a
slot of kernel memory; the host reads that slot and completes every fence up to the
value it finds, and a lost interrupt is recovered by reading it on a timer. A fence that
does not request the interrupt ends after dword 3.

The tail has no VM select and no barrier, so nothing in the stream orders the writeback
against the transfer or against the interrupt. GPIPE does both, and its writeback always
names domain 0.

> UNRESOLVED: whether the writeback retires when the engine issues it or when the
> transfer has drained. A signalled fence may therefore not prove the destination is
> readable by the CPU. A client frame on this ring is in the same position: its buffer
> packet carries a VMID while the fence writes a kernel address with no domain select,
> and no source states which domain that write uses.

## Open questions

- The state config word is fixed at `0x0080_0001`; its `8` in bits 23:20 matches the
  payload count, but no field definition states the split, so a driver MUST emit the
  constant.
- Whether a copy with element size `0` is accepted. The definition fixes the copy at
  eight-byte elements, while a client copying a shape with only 4-byte alignment emits
  element size `0` with operation `1`; that form is unestablished.
- What UMAP does to an address that is neither a device address nor translatable; only
  the VRAM-aperture test is implemented. Likewise for an unaligned address or pitch, or
  a pitch shorter than the row length.
- Whether ring-only packets — register write, poll, writeback, interrupt — are accepted
  inside a DMA buffer, and what the engine reports for a word that is not a packet. No
  interrupt source is defined for an illegal DMA command on this part.
- What a copy whose two ranges overlap does. The interface never emits one and copies
  through a third range instead, so the engine's contract is unestablished.
