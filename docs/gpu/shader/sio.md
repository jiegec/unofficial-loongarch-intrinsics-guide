# SIO

SIO (shader input/output) is the interface shaders use to exchange data with
the graphics pipeline: vertex position and varying export, fragment color
export, geometry input windows and emit, and streamout.

## Streams

| Stream | Role |
|--------|------|
| SIO0 | vertex output: position record and varyings |
| SIO1 | fragment color output / geometry stream |
| SIO2 | streamout / additional streams in GS |

## Instructions

| Instruction | Role |
|-------------|------|
| `siowrrot` / `siowrrot_imm` | write channel data to a stream with encoded rotation |
| `siowrrot_id` / `siowrrot_id_c4` | write to an explicit stream identifier |
| `siordrot` / `siordrot_imm` | read channel data with encoded rotation |
| `siordrot_id` / `siordrot_id_c4` | read from an explicit stream identifier |
| `sioread` / `sioread_imm` | read channel data |
| `siowrite` / `siowrite_imm` | write channel data |
| `sioexport` | commit a channel count from a stream to the stage output interface |
| `v_emit2` / `v_emit3` | emit a geometry vertex (GS) |

Channel addressing is either immediate (`siowrrot_imm`, `siowrrot_imm_c4`) or
register-based; the `_c4` forms transfer four channels in one operation.

## Usage by stage

| Stage | Stream | Pattern |
|-------|--------|---------|
| DS | SIO0 | write position at `0x0000` and varyings at `0x0200..`, then `sioexport` |
| PS | SIO1 | write four color channels, then `sioexport` |
| GS | SIO1 | read the input window (`siordrot_id_c4`), write multi-varying records, emit with `v_emit2/3`, update the stream header, `sioexport` |
| SS | memory | store through the streamout descriptor at `W + 0x20` |

## Completion

An export is committed with `sioexport`; the stage then terminates with
`check` and `exit`. The `check` row applies pending K/VM/VS/shared-memory
work, and `exit` completes the invocation.

## Hardware status

SIO file semantics are not fully closed. The file appears to require streamout
state to stabilize, and write-to-read round-trips are timing-sensitive; the
value transformation of `siowrite` is UNRESOLVED. The architectural contracts
above describe the intended stage usage, not a hardware-verified SIO file
model.
