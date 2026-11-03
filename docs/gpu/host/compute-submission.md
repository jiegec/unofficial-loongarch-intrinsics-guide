# Compute Submission

A compute submission uses a user queue and a 64-byte AQL kernel-dispatch packet.
This chapter defines the queue geometry, the kernel descriptor, the implicit
argument block and the completion path. The CPIPE management packets are in
[CPIPE](cpipe.md); the compute shader's view is in
[Compute](../shaders/compute.md).

## User queue

A user queue is created with `USERQ`:

| Field | Requirement |
|-------|-------------|
| Ring size | Power of two, 4 KiB to 1 MiB. |
| Ring address | 64-byte aligned GPU virtual address. |
| Index block | 64 bytes immediately before the ring. |
| Write index | 64-bit byte offset at `queue_va - 64`. |
| Read index | 64-bit byte offset at `queue_va - 56`. |

The driver writes complete AQL packets into the ring, stores the new write
pointer, and rings the doorbell. The kernel does not parse the ring. The queue's
MQD is mapped at creation and its doorbell is forwarded to the firmware as an
HIQ `DOORBELL` packet.

## AQL dispatch packet

The packet is 64 bytes, little-endian:

| Offset | Size | Field | Value |
|-------:|-----:|-------|-------|
| `0x00` | 2 | Header | `0x0b02` |
| `0x02` | 2 | Setup | `0x0001` |
| `0x04` | 2 | Workgroup size X | 1–65535 |
| `0x06` | 2 | Workgroup size Y | 1–65535 |
| `0x08` | 2 | Workgroup size Z | 1–65535 |
| `0x0a` | 2 | Reserved | 0 |
| `0x0c` | 4 | Grid size X | Workgroup count |
| `0x10` | 4 | Grid size Y | Workgroup count |
| `0x14` | 4 | Grid size Z | Workgroup count |
| `0x18` | 4 | Private segment size | Per-invocation scratch bytes |
| `0x1c` | 4 | Group segment size | Workgroup LDS bytes |
| `0x20` | 8 | Kernel object | GPU VA of the 96-byte kernel descriptor |
| `0x28` | 8 | Kernarg address | GPU VA of the argument block |
| `0x30` | 8 | Reserved | 0 |
| `0x38` | 8 | Completion signal | Optional GPU VA |

A zero workgroup dimension is malformed and is rejected. A zero grid dimension
is a legal empty dispatch.

## Kernel descriptor

The kernel descriptor is a 96-byte record. Its address, not the shader's entry
address, is the AQL packet's `kernel_object`.

| Offset | Size | Field | Meaning |
|-------:|-----:|-------|---------|
| `0x00` | 4 | Flags | `0x0fac`: enables the kernarg pointer and the group-id window. |
| `0x04` | 2 | Group size X | Local workgroup size. |
| `0x06` | 2 | Group size Y | Local workgroup size. |
| `0x08` | 2 | Group size Z | Local workgroup size. |
| `0x0a` | 2 | Reserved | 0 |
| `0x0c` | 4 | Kernel size X | Grid size in workgroups. |
| `0x10` | 4 | Kernel size Y | Grid size in workgroups. |
| `0x14` | 4 | Kernel size Z | Grid size in workgroups. |
| `0x18` | 4 | Private size | Per-invocation scratch bytes. |
| `0x1c` | 4 | Shared size | Workgroup LDS bytes. |
| `0x20` | 8 | Entry | GPU VA of the first instruction. |
| `0x28` | 8 | Kernarg address | Zero when the kernarg pointer is supplied by the packet. |
| `0x30` | 4 | Granulated counts | SGPR and VGPR counts in 128-byte granules. |
| `0x34` | 4 | Resource word | Kernel SGPR count in `[4:1]`, group SGPR count in `[7:5]`, floating-point mode in `[21:14]`. |

The flag word enables `s[6:7]` as the kernarg pointer. The kernel SGPR count
places the group ids: the compiler reads `group_id_x` from
`s[kernel_sgpr_cnt + 0]`, and the launch declares a count of 11. The group SGPR
count is 3.

## Implicit arguments

The kernarg block is reached through `s[6:7]`. The driver stages a fixed
implicit block after the user arguments:

| Offset | Size | Field |
|-------:|-----:|-------|
| `0` | 4 | Block count X (grid size in workgroups). |
| `4` | 4 | Block count Y. |
| `8` | 4 | Block count Z. |
| `12` | 2 | Group size X. |
| `14` | 2 | Group size Y. |
| `16` | 2 | Group size Z. |
| `18` | 2 | Remainder X. |
| `20` | 2 | Remainder Y. |
| `22` | 2 | Remainder Z. |
| `24` | 2 | Grid dimensions. |
| `32` | 8 | Global offset X. |
| `40` | 8 | Global offset Y. |
| `48` | 8 | Global offset Z. |

The local invocation id is in `v0/v1/v2`, one value per lane. The group id is in
the SGPR window described above. The block is 64 bytes, including the unused
bytes at `26..31`.

## Segments

Compute uses two segment apertures:

| Segment | Base | Size | Source |
|---------|------|------|--------|
| LDS | `0x0001_0000_0000_0000` | 4 GiB | AQL `group_segment_size` and the descriptor's shared size. |
| Scratch | `0x0002_0000_0000_0000` | 4 GiB | AQL `private_segment_size` and the descriptor's private size. |

The LDS and scratch bases are fixed. A client buffer MUST NOT be mapped inside
either aperture. The kernel descriptor's shared size is a byte count; the
granulated field can represent at most 262,016 bytes.

> UNRESOLVED: the per-workgroup LDS capacity, the maximum private segment per
> invocation, and whether the AQL completion signal is honoured on LG200.

## Local-size and dispatch records

Two fixed records describe a compute launch to the firmware. The current AQL
path does not use them; they are documented because a queue built around the
older record pair still reaches the same CPIPE.

A **Compute Local Size** record is 8 bytes:

| Bits | Field | Meaning |
|------|-------|---------|
| `[31:0]` | Command | `0x00100189`. |
| `[41:32]` | Local size X | 10-bit workgroup extent. |
| `[51:42]` | Local size Y | 10-bit workgroup extent. |
| `[61:52]` | Local size Z | 10-bit workgroup extent. |

A **Compute Dispatch** record is 16 bytes:

| Bits | Field | Meaning |
|------|-------|---------|
| `[31:0]` | Command | `0x00000787`. |
| `[63:32]` | Dimension 0 | Grid extent. |
| `[95:64]` | Dimension 1 | Grid extent. |
| `[127:96]` | Dimension 2 | Grid extent. |

The order of the three dimension fields is not implied by the command word; each
is programmed independently.

## Floating-point mode

The resource word's floating-point mode has four 2-bit fields: rounding for
32-bit operations, rounding for 16- and 64-bit operations, denormal handling for
32-bit operations and denormal handling for 16- and 64-bit operations.

| Denormal value | Meaning |
|---------------:|---------|
| 0 | Flush source and result. |
| 1 | Flush result only. |
| 2 | Flush source only. |
| 3 | Do not flush. |

The graphics pipe has no per-dispatch floating-point mode; it flushes denormals
natively. A compute shader that requires a different mode gets it from this
field.

## Completion

The AQL packet may carry a completion signal, or the submission may use the
CPIPE end-of-pipe interrupt and a fence. A user queue's completion is reported
through the same fence mechanism as a graphics submission.

## Open questions

- The exact behaviour of the completion signal and its ordering against the
  fence writeback.
- Whether a user queue may contain more than one dispatch in flight.
- The maximum grid size per dimension and the maximum workgroup size for each
  axis.
- The scratch-memory layout and the per-invocation stride.
