# Descriptors

The root fields in a shader-info record point to descriptor memory. They do
not normally point directly to image pixels or raw constant bytes.

```text
R = resource_sampler_descriptors

R + 0x00  +-------------------------------+  32-byte image descriptor
          | image base / dimensions / fmt |
          +-------------------------------+
R + 0x20  +-------------------------------+  16-byte gap in the combined record
          | no common descriptor meaning  |
          +-------------------------------+
R + 0x30  +-------------------------------+  16-byte sampler descriptor
          | filter / wrap / LOD / compare |
          +-------------------------------+

C = const_buffer
C + 0x00  +-------------------------------+  16-byte buffer descriptor
          | base address / byte extent    |
          | stride / format and control    |
          +-------------------------------+
```

| Descriptor | Size | Shader tuple | Fixed field contract |
|------------|-----:|--------------|----------------------|
| Buffer | 16 bytes | Four SGPRs (`RJ8x4`, `RK8x4`, or `RA8x4`) | Byte extent occupies descriptor bits `95:64`; it is a byte count |
| Image | 32 bytes | Eight SGPRs (`RK8x8` or `RA8x8`) | Image base, dimensions, format, type, and layout fields |
| Sampler | 16 bytes | Four SGPRs | Filter, wrap, LOD, comparison, and border controls |
| Attribute | 16 bytes per entry | Four SGPRs | Vertex fetch base, byte extent, stride, and format controls |

## Buffer descriptor (16 bytes)

| Bytes | Field | Meaning |
|------:|-------|---------|
| `0:63` | base address | GPU VA64 of the buffer payload |
| `64:95` | byte extent | byte count of the buffer |
| `96..127` | stride / format / control | stride, format, and control bits |

The byte extent is a byte count, not an element count. Tuple width must match
the descriptor record size (four SGPRs).

## Image descriptor (32 bytes)

| Bytes | Field | Meaning |
|------:|-------|---------|
| `0:63` | base address | GPU VA64 of the image payload |
| `64..95` | dimensions | width / height / depth or array size |
| `96..127` | format / type / layout | hardware format, surface type, tiling, swizzle |
| `128..255` | extended layout | mip/layer bounds and modifiers |

The exact bit assignment of the extended layout is not published; the format
and surface-type vocabulary matches the [surface records](../graphics/depth-blend.md#color-and-depth-surfaces).

## Sampler descriptor (16 bytes)

| Bytes | Field | Meaning |
|------:|-------|---------|
| `0:63` | filter | min/mag/mip filter selectors |
| `64:95` | wrap | U/V/W wrap modes |
| `96:111` | LOD | min/max LOD and LOD bias |
| `112:127` | compare and border | comparison function and border color |

Individual bit assignments are not published beyond these roles.

## Attribute descriptor (16 bytes per entry)

| Bytes | Field | Meaning |
|------:|-------|---------|
| `0:63` | base address | vertex-fetch base GPU VA64 |
| `64:95` | byte extent | fetch range in bytes |
| `96..127` | stride / format / control | element stride and format controls |

## Combined footprint and undefined intervals

The combined image/sampler footprint is at least `0x40` bytes. The `+0x20`
interval in the combined record has no common descriptor meaning. An array
stride is a separate pipeline contract and is not implied by the minimum
footprint. Unobserved descriptor formats remain unassigned.
