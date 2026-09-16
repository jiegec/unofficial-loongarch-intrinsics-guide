# Draw and Raster

## Draw Info

Draw Info is 36 bytes at state offset `0x000`:

| Bits | Field | Values / meaning |
|-----:|-------|------------------|
| `2:0` | Index Size | `0` no index buffer, `2` 16-bit, `4` 32-bit |
| `11:8` | Primitive Type | the primitive enum below |
| `16` | Primitive Restart | producer-controlled enable |
| `17` | Reset Primitive ID | reset primitive ID at draw start |
| `18` | Primitive Start Offset | producer-controlled start mode |
| `63:32` | Count | element/vertex count |
| `95:64` | Instance Count | instance count |
| `127:96` | Vertices Per Patch | tessellation patch size |
| `159:128` | Vertex Offset | API first-vertex/vertex-offset value |
| `191:160` | Restart Index | index restart value |
| `223:192` | Index Buffer Low | index GPU VA low word |
| `255:224` | Index Buffer High | index GPU VA high word |
| `287:256` | Primitive ID Input | primitive-ID seed/input |

Bits `7:3`, `15:12`, `19`, and `27:20` are not assigned by the reviewed
schema. Primitive restart behavior is separate from the two restart bits; a
producer may split a restart draw on the CPU.

The LG200 primitive enum:

| Value | Primitive | Value | Primitive |
------:|-----------|------:|-----------|
| `0` | point list | `7` | quad list |
| `1` | line list | `8` | quad strip |
| `2` | line loop | `9` | polygon |
| `3` | line strip | `10` | line list adjacency |
| `4` | triangle list | `11` | line strip adjacency |
| `5` | triangle strip | `12` | triangle list adjacency |
| `6` | triangle fan | `13` | triangle strip adjacency |

The triangle strip/fan values and the primitive table are LG200 values. A
driver must not import a comparison-generation enum without checking this
table.

## Pipe Enables

Pipe Enables occupies 12 bytes at state offset `0x030`:

| Bits | Field | Meaning |
-----:|-------|---------|
| `0` | Flat Shade | flat interpolation mode |
| `1` | Flat Shade First | provoking-vertex selection |
| `2` | Pipe Count | query/streamout pipe-count participation |
| `3` | Line Stipple | line stipple enable |
| `4..5` | Front/Back Face Cull | face culling |
| `6..7` | Early Z/Hi Z | depth early/hi-Z controls |
| `8` | Front CCW | winding convention |
| `9` | Bottom Edge Rule | bottom-edge raster rule |
| `10..11` | Raster Order | raster order selector |
| `12..13` | Fill Front | `0=fill`, `1=line`, `2=point` |
| `14..15` | Fill Back | same enum |
| `16..18` | Y controls | flip Y, coordinate Y, invert vertex Y |
| `19` | Sprite Origin | sprite coordinate origin |
| `20..22` | Polygon Offset Enables | point/line/fill offset enables |
| `23` | Window Rectangle Mode | window-rectangle mode |
| `24..25` | Edge Flag | edge flag and enable |
| `26` | Fixed Point Size Enable | fixed point size control |
| `27` | Stencil Write | stencil write participation |
| `28..30` | MRT Count | count-minus-one: 1 for two, 3 for four targets |
| `47..32` | Clip/Cull Plane Enable | eight-bit clip and cull masks |
| `48` | MSAA Enable | multisample enable; not the sample count |
| `49..51` | Smooth modes | line, point, polygon smooth |
| `52..55` | Raster Scan | scan selector |
| `56..57` | Sample Rate | sample-rate selector |
| `60` | Integer Coordinates | pixel-center integer mode |
| `64..75` | Layer | render-layer value |
| `76..79` | Viewport Index | viewport selector |
| `80..81` | Layer/Viewport Enable | enable corresponding system outputs |

Bits `31`, `58..59`, and `61..63` are not assigned by the reviewed schema.

## Viewport and guardband

Each of 16 viewport records is 40 bytes (`0x048 + 0x28*n`):

| Offset | Size | Field | Meaning |
|-------:|-----:|-------|---------|
| `+0x00` | 4 | X scale | float bit pattern |
| `+0x04` | 4 | X offset | float bit pattern |
| `+0x08` | 4 | Y scale | float bit pattern |
| `+0x0c` | 4 | Y offset | float bit pattern |
| `+0x10` | 4 | Z scale | float bit pattern |
| `+0x14` | 4 | Z offset | float bit pattern |
| `+0x18` | 2 | scissor min X | u16 |
| `+0x1a` | 2 | scissor max X | u16 |
| `+0x1c` | 2 | scissor min Y | u16 |
| `+0x1e` | 2 | scissor max Y | u16 |
| `+0x20` | 4 | depth min | float bit pattern |
| `+0x24` | 4 | depth max | float bit pattern |

The guardband at `0x308` contains four float bit patterns: min X, max X,
min Y, max Y.

## Line Raster

Line Raster is 12 bytes at state offset `0x318`:

| Offset | Size | Field | Meaning |
|-------:|-----:|-------|---------|
| `+0x00` | 1 | stipple factor | stipple repetition factor |
| `+0x01` | 2 | stipple pattern | 16-bit stipple bit pattern |
| `+0x03` | 1 | point size | quarter pixels |
| `+0x04` | 1 | line width | quarter pixels |

The remaining bytes of the record are not assigned by the reviewed schema.
