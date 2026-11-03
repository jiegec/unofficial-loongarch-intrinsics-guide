# Multisampling

LG200 supports at most four samples per pixel. A multisampled target is one surface whose
samples are interleaved inside its tiles: not four planes, not a compressed form, and with no
companion allocation. A resolve produces the single-sample image, and alpha-to-coverage is
computed by the fragment shader.

## Sample count

The framebuffer, depth/stencil and sampled-image sample counts are 1 and 4.

| Count | Surface storage | Surface Type |
|------:|-----------------|--------------|
| 1x | one storage word per texel | 0 (`2D`), 4 (`Layered`) |
| 4x | four storage words per texel, interleaved | 8 (`2D Multisample`), 12 for a colour layer range |
| 2x | no layout | — |

A storage word is the format's stored width, which can be wider than the exposed one: a depth
surface uses four bytes per texel even when it presents 16 bits. A 4x target is single-level,
keeps the level fields at 0, sets Tiling Mode 2, and sets Alignment to `log2(256 / storage
bytes)`: 6 for a four-byte colour texel, 5 for eight, 4 for sixteen. A depth or stencil target
always carries 6, which is the same rule because its storage word is four bytes. Alignment is
not a multisample flag: a single-sample surface of the same format carries the same value.

Nothing in the command stream distinguishes 2x from 4x. `Surface Type` 8 covers every
multisampled attachment and both counts serialize byte-identical surface words; no field in the
512-dword state object or in the indirect buffer carries the count. Only two per-draw fields
describe multisampling:

| Bits | Field | Meaning |
|------|-------|---------|
| Pipe Enables 48 | MSAA Enable | Set for a multisampled draw, clear for a single-sample one. Does not carry the count. |
| Pipe Enables 56:57 | Sample Rate | Per-pixel shading passes minus one, honoured only while MSAA Enable is set. A shading rate, not the sample count. |

Which field, if any, tells the rasteriser how many samples a target has is **UNRESOLVED**, and no
path describes a 2x attachment; a host MUST NOT issue a 2x draw. The one descriptor that names the
4x geometry is the image descriptor of a *sampled* multisample image: Image Type 14
(`2D Multisample`) or 15 (`2D Multisample Array`) with Last Level 2, the level count of the
multisample layout rather than a mip range; 0 there makes every per-sample fetch read the wrong
level.

## Colour layout

A 4x colour surface keeps the tile2 layout with the sample dimension folded into each tile: a
texel owns four consecutive storage words, one per sample. A layer spans
`ceil(height / 4) * (pitch / 4) * 64 * storage_bytes` bytes, where `pitch` is the surface word's
row pitch in texels of the storage word; the depth map below uses the same span, and the map is
the whole surface, so no second buffer holds coverage. Because four samples share each word
group, a full clear can write the allocation as one flat run of storage words, but a partial one
has to walk the sample map.

The placement of a region inside that span is **UNRESOLVED**: one reading puts the region of
texel `(x, y)` at `(y / 4) * (pitch / 4) * 0x100 + (x / 4) * 0x100`, the depth map without the
row inversion; the other gives a colour tile a plane-and-slot address inside eight 0x2000-byte
segments, leaves two tiles of the 8x8 tile grid unused, and rotates the channels with two tile
exceptions. A host MUST NOT read a drawn 4x colour surface as a linear image and MUST resolve it
on the GPU.

## Depth and stencil layout

The depth map is the one 4x layout whose address arithmetic is established: a grid of 256-byte
regions, each holding a 4x4 group of texels with their four samples. The rows are inverted:
window row `y` is surface row `h = height - 1 - y`, and the map is indexed by `h`. Within a
region the word of sample `s` of texel `(x, h)` is

```text
w = ((h % 4) >= 2 ? 32 : 0) + ((x % 4) >= 2 ? 16 : 0)
  + (h & 1 ? 8 : 0) + (x & 1 ? 4 : 0) + s
```

and the region starts at `(h / 4) * group + (x / 4) * 0x100` with `group = (pitch / 4) * 0x100`;
both byte terms scale by the storage width. The same formula with four-byte constants is in
[Formats and layout](formats.md#multisample-layout).

## Resolve

| Source | Path | Restriction |
|--------|------|-------------|
| Colour | fragment pass: fetch each of the four samples, write one texel per pixel | integer colour formats are single-sample only |
| Depth | CPU mapping of the sample map, read sample 0 | minimum, maximum and average need their own support |
| Stencil | CPU mapping, aspect merged word by word | |

A resolve is not a blend: it builds the destination texel from the source samples and does not run
the attachment's blend equation. Nor is it a copy, because the samples share a tile. A 4x-to-4x
copy MUST move all four sample words; a path that stages sample 0 and writes one value into all
four words implements the resolve contract and destroys samples 1 to 3.

## Alpha-to-coverage

Alpha-to-coverage requires a 4x target. A draw that requests it on a single-sample target MUST be
rejected, and an integer colour attachment MUST be rejected with it.

There is no fixed-function alpha-to-coverage state: the fragment shader computes the mask. Per
covered fragment it clamps the exported alpha to 0..1, scales by 4, rounds to nearest, forms
`(1 << n) - 1`, and ANDs the result with the sample mask, so alpha 0 keeps no sample and alpha 1
keeps all four. The mask reaches the rasteriser through the coverage channel; the fragment input
config declares it with its `Sample Mask Output` bit (bit 44).

A shader that needs no explicit mask MUST NOT write one: the full 0xf mask selects a coverage path
that drops part of a 4x colour surface, while an implicit mask uses the rasteriser's own coverage.
A sample mask other than 0xf still has to be exported without alpha-to-coverage, and alpha-to-one
applies after alpha-to-coverage.

## Open questions

- The field that names the sample count, and therefore whether 2x is reachable at all.
- The 2x sample layout and its layer span.
- The four sample positions, and which order a per-sample read addresses them in.
- Whether the depth map's four-row group stride is fixed at 0x2000 bytes or follows the row pitch;
  the two readings differ for a pitch other than 128 texels.
