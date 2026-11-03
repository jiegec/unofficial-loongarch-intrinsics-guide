# Shader Stages

A graphics pipeline has five stage slots and four programmable roles. This
chapter defines the slots, the stage roles and the interfaces between them. The
record that describes a stage is in [Shader ABI](shader-abi.md); the registers a
stage receives are in [Registers](registers.md).

## Slots

Six 0x60-byte records start at state offset `0x540`:

| Slot | Offset | Shader type | Role |
|-----:|-------:|------------:|------|
| 0 | `0x540` | — | Vertex slot; not an execution slot. |
| 1 | `0x5a0` | — | Hull slot; not programmable. |
| 2 | `0x600` | 3 | Domain slot; carries the ordinary vertex role. |
| 3 | `0x660` | 4 | Geometry. |
| 4 | `0x6c0` | 6 | Streamout. |
| 5 | `0x720` | 5 | Fragment. |

The ordinary vertex role is carried by slot 2 with the domain-as-vertex bit set,
not by slot 0. Slot 0 and slot 1 are never execution slots. Slot 0 holds a
prefetch-safety copy of the domain record when a draw runs.

## Stage roles

| Stage | Programmable | Inputs | Outputs |
|-------|--------------|--------|---------|
| Vertex | Yes, in slot 2 | Vertex and instance ids, attribute table, descriptor roots | SIO0 record |
| Geometry | Yes, slot 3 | SIO0 record, six vertex selectors, primitive id | SIO1 record |
| Fragment | Yes, slot 5 | Interpolated varyings, barycentrics, fragment coordinate | SIO1 colour and depth |
| Streamout | Yes, slot 4 | Destination descriptors and a record count | Buffer writes |
| Hull | No | — | — |

There is no hull shader and no tessellation. A patch primitive is rejected
before state is built.

## Selection

The shader config word enables the slots:

| Bit | Stage |
|----:|-------|
| 0 | Vertex slot |
| 1 | Hull slot |
| 2 | Domain slot |
| 3 | Geometry slot |
| 4 | Streamout slot |
| 5 | Domain as vertex |
| 6 | Domain includes geometry |
| 7 | Vertex includes hull |

A vertex-only draw sets domain enable and domain-as-vertex. A draw with geometry
sets domain enable, geometry enable and domain-as-vertex. A draw with streamout
sets streamout enable and installs slot 4.

## Stage interfaces

The stages exchange data through SIO and through descriptor-bound memory. The
SIO streams are:

| Stream | Producer | Consumer |
|--------|----------|----------|
| SIO0 | Vertex | Geometry, or the rasterizer when no geometry runs |
| SIO1 | Geometry or fragment | Rasterizer or output merger |
| SIO2 | No documented use | — |

The SIO channel layout, the export count and the emit/cut rules are in
[SIO](io.md).

## Entry and exit

A stage starts at the kernel entry in its shader-info record. It captures the
active lane mask, installs a whole-quad mask if it uses helper lanes, runs its
body, writes its outputs, commits the export with the export instruction, orders
pending work, and terminates.

The entry registers per stage are in [Registers](registers.md); the exit order is
in [Shader ABI](shader-abi.md#entry-and-exit).

## Consistency

A stage is valid only when these agree:

| Item | Must match |
|------|------------|
| Shader-info size fields | What the program uses |
| Code size | The uploaded binary length |
| Kernel entry | The first instruction of the binary |
| Export count | The channels the stage produced |
| Fragment attribute map | The producer's output ordinals |
| Fragment input config | The inputs the program reads |
| Interpolation coefficient count | The coefficient vectors the program reads |
| Stage config records | The stage topology actually enabled |
| Descriptor roots | The tables the program loads from |

## Open questions

- The exact role of the vertex slot's prefetch copy on every launch shape.
- Whether the streamout slot can run as a graphics stage.
- The behaviour of a draw that enables two mutually exclusive slots.
