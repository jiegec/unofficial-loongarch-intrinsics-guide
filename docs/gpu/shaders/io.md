# SIO

SIO is the channel-addressed interface between a shader and the fixed-function
stages around it. It carries vertex output, geometry input and output, and
fragment colour without going through memory. This chapter defines the streams,
the channel layout, the transfer instructions and the ordering rules.

## Streams

| Stream | Carries |
|--------|---------|
| SIO0 | Vertex output; the geometry stage reads its input from the same ring. |
| SIO1 | Geometry output and fragment colour output. |
| SIO2 | No documented use. |

A stream is a ring of 32-bit channels. A producer writes channels and commits
them with an export; a consumer reads them back. A transfer names a stream, a
channel slot and a window size.

## Channels

| Channels | Content |
|----------|---------|
| 0–3 | Position, four components. |
| `position_channels + 4 * b + c` | Component `c` of generic varying ordinal `b`. |
| Distance payload | A position-adjacent block; see [Geometry](geometry.md). |

A window is described by the instruction. The window width is part of the record
layout; a producer and its consumer MUST agree on it. A larger consumer window is
not automatically compatible: it moves every read to a different ring record.

## Instructions

| Group | Instructions |
|-------|--------------|
| Write a channel | `siowrite`, `siowrite_imm`, `siowrrot`, `siowrrot_c4`, `siowrrot_id`, `siowrrot_id_c4`, `siowrrot_imm`, `siowrrot_imm_c4` |
| Read a channel | `sioread`, `sioread_imm`, `siordrot`, `siordrot_c4`, `siordrot_imm`, `siordrot_imm_c4`, `siordrot_id`, `siordrot_id_c4` |
| Commit an export | `sioexport` |
| Emit a geometry vertex | `v_emit2`, `v_emit3` |

The `_c4` forms move four channels through an aligned four-register tuple. Every
other form moves one channel through one register.

## Operand roles

| Field | Role |
|-------|------|
| `VJ8` | Data register: the value written or the destination of a single-channel read. |
| `VK8` | Channel slot, held in a register. |
| `M.chan12` | Channel slot, as an immediate. |
| `M.channum12` | Window size in channels. |
| `IA16`, `IK16` | Window offset: which output plane the transfer addresses. |
| `M.sioid1` | Stream selector, 0 or 1. |
| `VA8` | Ring channel index, held in a register. |
| `PJ2` | Mask selector. |

## Writing

A write takes its data from the register named by `VJ8` and places it in the
channel slot named by `VK8` or `M.chan12`, inside the window selected by the
offset field.

The channel slot is also a descriptor slot. Before a write, the register whose
number equals the slot MUST hold the record descriptor for that channel; the
hardware reads the descriptor from that register rather than from an operand.
A producer stages the descriptor with a register move, writes the data, and
orders. A write whose slot register holds data leaves the channel with the wrong
record type.

## Reading

The identifier read form `siordrot_id_c4` is the geometry input read. It takes
the ring channel index from `VA8`, the input-vertex selector from `VK8` and the
window size from `M.channum12`.

Its destination is not the encoded destination field. A four-channel read writes
the four VGPRs that begin at the next entry of a fixed sequence:

```text
4, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 8
```

The sequence holds fifteen reads and then wraps. A consumer that needs more than
fifteen open reads MUST copy each read out of the window before issuing the next
one.

## Export

`sioexport` commits an export count from one stream to the stage output. The
count need not equal the window width; the window is the transfer size and the
count is the number of records committed.

- A vertex stage exports position channels plus varying channels.
- A geometry stage rewrites its header slot with the emitted-vertex count before
  the export; its export count is the encoded record quantity.
- A fragment stage exports its colour channels plus one when it writes depth. A
  fragment stage that writes nothing still exports one channel.
- A vertex stage fused into a geometry stage does not export at all.

An export whose count does not match the staged data leaves channels unexported
or commits positions that were never written. An export whose header slot was
not updated commits an empty stream that never reaches the rasterizer.

## Emitting

`v_emit3` and `v_emit2` emit the vertex staged in the output window. They take
the emit index through `GK11`, the record argument through `GA11` and a
separator register selector in `VJ8`.

The two opcodes select the stream shape: `v_emit2` for point and line streams,
`v_emit3` for triangle streams. Emitting a line stream with `v_emit3` loses the
first segment of every strip, because the hardware assembles triangle records
and only a cut resets them. The cut is a separate step: window maintenance
masks and writes the separator, restores the active lane mask, advances the
vertex counter and orders before the next emit.

## Ordering

1. An ordering operation follows every write and precedes every export.
2. Reads are issued in ascending channel order.
3. An emit is followed by an ordering operation before the next emit stages its
   payload.
4. A vertex MUST NOT reuse a slot register for its own payload.
5. The header slot is updated with the emitted count before the final export.
6. Exports are issued in ascending output order.
7. A stage ends with writes, the export, the ordering operation and the
   terminator.

A read that does not move forward in the ring stalls every later export of the
stage. Reads are issued in ascending channel order even when the program loads
its inputs in another order.

## Open questions

- Whether an SIO transfer is gated by the mask selector like an ALU operation.
- The exact difference between the `write` and `rot` spellings.
- The meaning of the offset field's special payload class.
- SIO2.
- Whether the stream interface needs state to settle before a read observes a
  write.
