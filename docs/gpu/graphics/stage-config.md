# Stage Configuration

## Shader Config

Shader Config is 8 bytes at state offset `0x4c0`:

| Bits | Field | Meaning |
|-----:|-------|---------|
| `0` | VS_ENABLE | select VS slot |
| `1` | HS_ENABLE | select HS slot |
| `2` | DS_ENABLE | select DS slot |
| `3` | GS_ENABLE | select GS slot |
| `4` | SS_ENABLE | select SS slot |
| `5` | DS_AS_VS | use DS program for the ordinary vertex role |
| `6` | DS_INCLUDES_GS | fuse/route DS program with GS role |
| `7` | VS_INCLUDES_HS | fuse/route VS program with HS role |
| `12..15` | GS primitive type | geometry primitive |
| `16..17` | DS primitive type | tessellation primitive |
| `18` | DS point mode | point tessellation |
| `19` | DS clockwise | winding |
| `20..21` | DS spacing | tessellation spacing |
| `32..42` | DT maxima | DT vertex/primitive maxima and reuse disable |
| `48..58` | TT maxima | TT vertex/primitive maxima and reuse disable |

The enable bits select records; they do not rename one stage's calling
convention into another. The DT/TT maxima field layout is not closed; only
their presence is assigned.

## Stage config records

The five stage-config records describe inputs and output packing. Fields
marked UNRESOLVED are present but their exact layout is not closed.

### VS Stage Config (`0x4c8`)

| Field | Status |
|-------|--------|
| clip distance | assigned |
| edge/point/layer/viewport outputs | assigned |
| instance/wave inputs | assigned |
| position size | assigned |
| total size | assigned |
| attribute sizes | assigned |

### HS Stage Config (`0x4d0`)

| Field | Status |
|-------|--------|
| primitive ID / invocation / wave inputs | assigned |
| level size | assigned |
| total size | assigned |
| attribute sizes | assigned |

### DS Stage Config (`0x4d8`)

| Field | Status |
|-------|--------|
| primitive / tessellation / instance inputs | assigned |
| output flags | assigned |
| position size | assigned |
| total size | assigned |
| attribute sizes | assigned |

### GS Stage Config (`0x4e0`)

| Field | Status |
|-------|--------|
| invocations | assigned |
| stream count | assigned |
| maximum threads | assigned |
| input/output IDs | assigned |
| vertices | assigned |
| stride | assigned |
| total / attribute sizes | assigned |

### SS Stage Config (`0x4e8`)

The slot exists; detailed fields remain UNRESOLVED.

## Fragment state

### Fragment Attribute Map (`0x4f0`, 32 entries x 2 bytes)

| Bits | Field | Meaning |
|-----:|-------|---------|
| `4:0` | attribute ID | attribute number |
| `5` | generated | generated attribute |
| `6` | sprite texture | sprite texture flag |

### Fragment Input Config (`0x530`)

| Bits | Field | Meaning |
|-----:|-------|---------|
| `15:0` | input address | fragment input base |
| `31:16` | input enable | per-input enable mask |
| `32` | primitive ID output | system output enable |
| `33` | stencil output | system output enable |
| `34` | depth output | system output enable |
| `35` | vmask output | system output enable |
| `43:36` | coefficient count | interpolation coefficient count |
| `44` | sample-mask output | system output enable |
| `47` | scalar init alignment | scalar initialization alignment |

### Fragment Output Config (`0x538`)

The MRT format word. Individual bit meanings are not published beyond the
format vocabulary used by the surface records.

## Graphics-to-shader hand-off

The six `0x60`-byte shader-info slots are part of this graphical payload.
Their record fields, descriptor roots, stage live-ins, and SIO records are
defined in [Shader ABI](../shader-abi.md). The hand-off is therefore explicit:

```text
state + 0x4c0             stage enable/topology
state + 0x540 + 0x60*n    shader-info record n
record + 0x10             Kernel Entry GPU VA
record + 0x20..0x40       descriptor roots (rw_buffer, const_buffer, constant,
                          resource_sampler_descriptors, attributes)
record + 0x48             four stage arguments
```

A stage enable bit selects a slot; it does not rename another stage's calling
convention. Fixed-function state supplies interpolation, attachment, and
system-value configuration around the selected shader.

## Resource and query extensions

The graphical payload can reference buffer/image/sampler descriptors through
stage roots. Descriptor field layouts and binding-slot rules are specified in
[Shader ABI](../shader-abi.md), while attachment records define render-target
storage.

The reviewed command extension for queries uses a begin/end pair:

```text
QUERY_BEGIN: operation=13, selector
QUERY_END:   operation=12, selector, result_address64
```

Selectors include transform-feedback primitives written `0x200`, primitives
generated `0x2000`, and occlusion `0x400000`. Timestamp begin/end uses command
`0x13` with phase `1`/`0x200` and a result address. Streamout adds the SS stage
record and a streamout-end record; its complete vertex-data ring contract is
not merged into the generic shader-info table until the remaining stage fields
are closed.

## Defined versus unresolved fields

The tables above are the LG200 schema, not a promise that every API feature is
implemented by every producer. The state tail, the three unknown blocks,
complete tessellation limits, SS stage words, arbitrary MRT shader export
routing, sample-count storage, and unobserved descriptor formats remain
explicitly unresolved. A driver must preserve reserved bits and reject a
feature whose packet contract is not defined.
