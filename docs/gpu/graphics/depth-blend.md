# Depth, Stencil and Blend

## Depth Control

Depth Control is 16 bytes at state offset `0x324`:

| Offset | Size | Field | Meaning |
|-------:|-----:|-------|---------|
| `+0x00` | 4 | polygon offset units | float bit pattern |
| `+0x04` | 4 | polygon offset factor | float bit pattern |
| `+0x08` | 4 | polygon offset clamp | float bit pattern |
| `+0x0c` | 1 | depth enable | 0/1 |
| `+0x0d` | 1 | depth write | 0/1 |
| `+0x0e` | 1 | near/far clamp | clamp enable |
| `+0x0f` | 1 | compare function | 3-bit enum below |

Compare function:

```text
Never=0, Less=1, Equal=2, LessEqual=3,
Greater=4, NotEqual=5, GreaterEqual=6, Always=7
```

## Stencil

Front Stencil Face Control is at `0x334`, back at `0x338`; each is 4 bytes:

| Offset | Bits | Field | Meaning |
|-------:|-----:|-------|---------|
| `+0x00` | 0 | enable | 0/1 |
| `+0x00` | `3:1` | compare function | 3-bit enum as above |
| `+0x00` | `6:4` | fail operation | keep/zero/replace/inc/dec/... |
| `+0x00` | `9:7` | depth-pass operation | operation when depth passes |
| `+0x00` | `12:10` | depth-fail operation | operation when depth fails |
| `+0x01` | 0 | value mask | compare mask |
| `+0x02` | 0 | write mask | write mask |

Stencil operations are `keep=0`, `zero=1`, `replace=2`, `increment-clamp=3`,
`decrement-clamp=4`, `increment-wrap=5`, `decrement-wrap=6`, `invert=7`.

## Blend

Blend Control is 4 bytes at state offset `0x33c`:

| Bits | Field | Meaning |
|-----:|-------|---------|
| `0` | independent | enable per-RT blend words |
| `1` | dual source | dual-source blend mode |
| `3` | logic-op enable | logic operation gate |
| `7:4` | logic-op | 16 standard truth-table selectors `clear=0`..`set=15` |

When independent is clear, RT0's blend word is reused for every attachment;
when set, each RT word is honored.

Each Blend RT word (`0x340 + 4*n`, `n = 0..7`):

| Bits | Field |
-----:|-------|
| `0` | blend enable |
| `3:1` | RGB equation: add=0, subtract=1, reverse subtract=2, minimum=3, maximum=4 |
| `8:4` | RGB source factor |
| `13:9` | RGB destination factor |
| `16:14` | alpha equation |
| `21:17` | alpha source factor |
| `26:22` | alpha destination factor |
| `30:27` | R/G/B/A write mask |

The factor enum includes `one=1`, source/destination color/alpha `2..5`,
source-alpha-saturate `6`, constants `7..8`, source1 `9..10`, zero `17`, and
the one-minus forms `18..26`.

Blend Constants at `0x360` hold four 32-bit float bit patterns (constant
color for factor `7..8`).

## Color and depth surfaces

Each Color Buffer (`0x370 + 0x20*n`, `n = 0..7`) and the Depth Buffer
(`0x470`) is a 32-byte record:

| Offset | Bits | Field | Meaning |
|-------:|-----:|-------|---------|
| `+0x00` | 63:0 | base address | GPU VA64 |
| `+0x08` | `7:0` | hardware format | format enum below |
| `+0x08` | `11:8` | surface type | 2D/layered/mip/msaa enum below |
| `+0x08` | `15:12` | tiling | tiling mode |
| `+0x08` | `27:16` | swizzle | 12-bit component swizzle |
| `+0x08` | `31:28` | reverse/pow2 flags | layout flags |
| `+0x0c` | `31:16` | width | u16 |
| `+0x0c` | `15:0` | height | u16 |
| `+0x10` | `31:16` | depth | u16 |
| `+0x10` | `15:0` | pitch | u16 |
| `+0x14` | `11:0` | base layer | u12 |
| `+0x14` | `23:12` | last layer | u12 |
| `+0x14` | `27:24` | base level | u4 |
| `+0x14` | `31:28` | last level | u4 |
| `+0x18` | `3:0` | level | u4 |
| `+0x18` | `7:4` | alignment | u4 |

The reviewed surface-type enum is `2D=0`, `2D mip=2`, `layered=4`,
`layered mip=6`, `2D multisample=8`, and `2D multisample array=12`.
`2D multisample` covers both 2x and 4x in the current state word; the sample
count is not encoded by this field.

The color hardware-format table includes RGBA8 `0`, RGBA16 float `2`,
RGBA32 float `3`, signed/unsigned integer RGBA `4..9`, RG/R `10..19`,
R11G11B10 float `32`, sRGB RGBA8 `35`, and RGB10A2 `44`. Depth formats are
Z24 `37`, Z32 float `38`, Z24+stencil8 `39`, and Z32 float+stencil8 `40`.

## Unassigned fields

The following are explicitly not assigned by the reviewed schema: the draw
tail at `0x024`, the pipe tail at `0x03c`, the pipeline-config block at
`0x490` (0x30 bytes), the fragment tail at `0x53c`, and the state tail at
`0x780` (0x80 bytes). Sample-count storage and unobserved descriptor formats
remain unresolved.
