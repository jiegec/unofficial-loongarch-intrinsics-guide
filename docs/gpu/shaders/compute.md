# Compute

A compute shader is a kernel launched by CPIPE. It reads its position from
launch state rather than from a draw, and it has no constant root. This chapter
describes the shader's view. The host path is in
[Compute submission](../host/compute-submission.md).

## Launch

A compute launch is an AQL packet on a user queue. The packet supplies the
workgroup size, the grid size, the private and group segment sizes, the kernel
descriptor address and the kernarg address. The kernel descriptor supplies the
entry address and the register counts.

The compute pipe has no graphics constant root. The kernarg pointer arrives in
`s[6:7]`; the group ids arrive in `s11..s13`; the local invocation id arrives in
`v0..v2`.

## Implicit arguments

The kernarg block carries the grid and group geometry:

| Offset | Size | Field |
|-------:|-----:|-------|
| `0` | 4 | Block count X. |
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

The block is reached from the kernarg pointer, not from a constant root. The
layout is fixed; only the distance from the user arguments is a driver choice.

## Identifiers

| Identifier | Source |
|------------|--------|
| Local invocation id | `v0`, `v1`, `v2`. |
| Group id | `s[kernel_sgpr_cnt + 0..2]`. |
| Workgroup size | Kernarg block at +12, +14, +16. |
| Grid size | Kernarg block at +0, +4, +8. |
| Global offset | Kernarg block at +32, +40, +48. |
| Global id | Computed: group id * group size + local id. |

A kernel that reads a group id MUST declare at least 14 SGPRs, because the
group window starts at 11. The launch declares a kernel SGPR count of 11 and a
group SGPR count of 3.

## Shared memory

Workgroup shared memory is addressed through the LDS aperture. The kernel
descriptor's shared size is the byte count the workgroup allocates. The AQL
packet's group segment size is the same quantity for the launch.

The LDS aperture is 4 GiB. The descriptor's granulated field can represent at
most 262,016 bytes.

## Barriers and ordering

A workgroup barrier synchronises the invocations of one workgroup. It is the
ordering mechanism for shared memory: a store before the barrier is visible to
other invocations after it. The barrier is not a global memory fence.

The shader ordering instruction orders pending memory and interface work for the
current invocation. It is not a barrier.

## Floating-point mode

The kernel descriptor's resource word selects rounding and denormal handling for
the dispatch. The graphics pipe has no equivalent field and flushes denormals
natively. See [Compute submission](../host/compute-submission.md#floating-point-mode).

## Limits

| Limit | Value |
|-------|-------|
| Wave width | 32 lanes |
| SGPR window for group ids | 11 through 13 |
| Local size field | 16 bits in the kernarg block |
| Grid size field | 32 bits per dimension |
| LDS aperture | 4 GiB |
| Representable shared size | 262,016 bytes |

## Open questions

- The maximum workgroup size per axis and the maximum grid size per dimension.
- The scratch-memory layout and the per-invocation stride.
- Whether the AQL completion signal is honoured on LG200.
- Whether a compute kernel can use the graphics SIO interface.
