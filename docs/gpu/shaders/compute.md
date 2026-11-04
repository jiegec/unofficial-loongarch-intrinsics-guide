# Compute

A compute shader is a kernel the CPIPE runs over a grid of workgroups. It has no
draw state and no constant-root register: everything it learns about its place in
the grid arrives through a fixed scalar window, three per-lane registers, and an
implicit-argument block reached through the kernarg pointer. This chapter defines
those inputs, the arithmetic that turns workgroup geometry into waves, and the
rules for shared memory, barriers and floating-point mode. The queue that
publishes a dispatch is in
[Compute submission](../host/compute-submission.md).

## Launch

The CPIPE queue consumes an MCS (micro-code stream), not AQL dispatch records: a
well-formed AQL record at the write pointer is ignored. A dispatch is a fixed
sequence of state words carrying the geometry, the apertures and the two resource
words, a group command whose three counters are workgroup counts, and a tail that
waits, invalidates, signals and drains.

The launch record is a 96-byte kernel descriptor whose entry word points at the
first instruction: a dispatch names a descriptor, not code. Two parts of it
decide what the shader receives: the flags word and the two resource words.

The flags word `0x0fac` sets bits 2, 3, 5, 7, 8, 9, 10 and 11: a
private-segment descriptor, a dispatch pointer, the kernarg pointer, the
flat-private base, the private size, and the three group-id enables, which are
also the group command's three dimension enables. Bits 4 and 6 (queue pointer,
dispatch id) stay clear, so the launch does not configure those two inputs.

| Word | Bits | Field |
|------|------|-------|
| 0 | 5:0 | VGPR count in units of 4, rounded up. |
| 0 | 9:6 | SGPR count in units of 8, rounded up. |
| 0 | 20:10 | Shared window in 128-byte granules. |
| 1 | 4:1 | Kernel SGPR count: 11. |
| 1 | 7:5 | Group-id SGPR count: 3. |
| 1 | 21:14 | Floating-point mode. |
| 1 | 31:27 | Waves per workgroup. |

Geometry, entry, kernarg address, both segment sizes and both resource words are
rewritten per dispatch, because the firmware reads them when it launches the
waves. A descriptor left at its creation values describes a different launch.

## Implicit arguments

The grid and group geometry is a fixed 64-byte block written behind the kernel
arguments; only its distance from the kernarg pointer is a driver choice.

| Offset | Size | Field |
|-------:|-----:|-------|
| `0`, `4`, `8` | 4 each | Workgroup count X, Y, Z. |
| `12`, `14`, `16` | 2 each | Local size X, Y, Z. |
| `18`, `20`, `22` | 2 each | Remainder X, Y, Z (`UNRESOLVED`). |
| `24` | 2 | Grid dimensions (`UNRESOLVED`). |
| `26` | 2 | Padding. |
| `32`, `40`, `48` | 8 each | Dispatch base X, Y, Z, signed. |

The block is reached from the kernarg pointer in `s[6:7]`, not from a constant
root. The staging offset MUST be 16-byte aligned and MUST leave all 64 bytes
below the 16-bit offset limit of the wide scalar load. The block is read with
four 16-byte flat loads, so every 16-bit field MUST lie wholly inside one 4-byte
word, and the halves are extracted with a bitfield instruction. Push constants
are staged at a second chosen offset from the same pointer, in a window that does
not overlap the block.

The workgroup counts are what `get_num_groups()` returns. The local sizes are
consumed only when the size is not a compile-time constant, and the dispatch base
forms the workgroup id. Remainder and grid dimensions have no established
meaning: a launch writes zero there and nothing consumes them.

## Identifiers

| Identifier | Source |
|------------|--------|
| Workgroup id X, Y, Z | `s11`, `s12`, `s13`, plus the dispatch base. |
| Local invocation id X, Y, Z | `v0`, `v1`, `v2`, per lane. |
| Workgroup count | Implicit block at `+0`, `+4`, `+8`. |
| Local size | Implicit block at `+12`, `+14`, `+16`. |
| Dispatch base | Implicit block at `+32`, `+40`, `+48`. |
| Global invocation id | No input; derived as workgroup id × local size + local id. |

The hardware group-id counter always starts at zero. A dispatch base exists only
in the implicit block, and the shader adds it to each hardware id in 32-bit
arithmetic, so the sum wraps in 32 bits.

The group-id window is hardware-provided and sits outside the register count the
shader declares. The descriptor names its base (kernel SGPR count 11) and its
length (group count 3); those registers are reserved so no temporary is allocated
over them, and resource word 0's SGPR field reports only the ordinary high-water
mark. The kernarg pair `s[6:7]` is reserved the same way.

## Workgroups and waves

A wave is 32 lanes in this ABI. A workgroup of N invocations is ceil(N / 32)
waves, which the launch computes and carries in resource word 1 bits 31:27; a
workgroup larger than one wave is normal.

The local size reaches hardware through one word of the aperture state command:
three 10-bit fields, each holding the dimension minus one. A dimension is
therefore 1 through 1024, and a larger value MUST be refused rather than
truncated. A stream that omits that word runs one lane per workgroup regardless
of what the descriptor says.

The group command carries the workgroup counts, obtained by dividing the grid
size in work-items by the local size per dimension. A zero count in any dimension
starts no workgroup. Each counter is a 16-bit quantity on the wire, so a dispatch
with more than 65,535 workgroups in one dimension MUST be split; splitting X
preserves the Y and Z coordinates and the running dispatch base.

## Shared memory

Workgroup shared memory is an LDS window inside a 4 GiB aperture based at
`0x1_0000_0000`. A shared-memory instruction's address is a 32-bit byte offset
within that aperture; the immediate form carries 16 bits. The window belongs to
one workgroup, and the firmware sizes each workgroup's region from the byte count
in the launch.

The declared window travels in two places that MUST agree: the descriptor's
shared-size word at `+0x1c` and resource word 0 bits 20:10, which holds
ceil(bytes / 128) in 128-byte granules. Eleven bits name at most 2047 granules,
so the largest representable window is 262,016 bytes. A larger declaration MUST
be refused before the launch is built, because the field would fold it down to a
smaller window. A dispatch that declares a window while the group aperture is
zero is malformed.

The private segment is per invocation. Its size is the per-invocation byte count
times the product of the group counts and local sizes, rounded up to 64 KiB, and
it MUST cover the dispatch base as well, because an invocation's index is
`lane + group × Sx·Sy·Sz` with `group = wg.x + Gx·(wg.y + Gy·wg.z)` and `lane`
the linear local index. The base is staged as a 64-bit address in a hidden window
behind the push constants; a flat access adds no implicit private base, so every
scratch access is absolute.

## Barriers and ordering

A workgroup barrier synchronises the invocations of one workgroup and orders
workgroup-visible memory: a store before the barrier is visible to the other
invocations after it. Its scope is the workgroup; it is neither a global memory
fence nor a rendezvous for an arbitrary lane mask. Because a workgroup can span
several waves, the barrier cannot be replaced by a mask operation.

The ordering instruction drains the pending work named by four domain bits:
kernel and scalar (0), vector memory (1), vector store (2) and shared memory (3).
It does not signal completion, is not a host fence and is not a lane rendezvous;
it may be placed inside a narrowed mask region and orders that region's work. It
is the ordering primitive that reaches beyond one workgroup: shared memory
written by one workgroup and read by another MUST be separated by an ordering
instruction, while a workgroup barrier suffices inside one workgroup.

LG200 has no subgroup-scope execution barrier. A subgroup is one wave, whose lanes
run in lockstep, so a subgroup barrier carries only memory ordering.

Dependencies between GFX and CPIPE belong to host submission ordering. Completing
one CPIPE dispatch does not complete an earlier asynchronous GFX producer. In
the current loongvk path, the command executor completes flushed GFX work before
either a direct dispatch or the CPU read of indirect dispatch counts. It waits
before descriptor-alias preparation as well. See
[Host ordering between consumers](geometry.md#host-ordering-between-consumers)
for the graphics, compute and readback boundaries. These host waits do not
change the domain bits or semantics of a shader ordering instruction.

## Floating-point mode

Resource word 1 bits 21:14 hold four 2-bit fields: rounding for 32-bit values at
14:15, rounding for 16- and 64-bit values at 16:17, the denormal mode for 32-bit
values at 18:19, and the denormal mode for 16- and 64-bit values at 20:21.

The denormal mode is 0 for flushing both source and output, 1 for output only, 2
for source only, and 3 for no flush. Rounding mode 0 is round-to-nearest-even. The
mode is per dispatch, and the graphics pipe has no equivalent field: it flushes
denormals natively.

A shader that requires denormals to be flushed gets 0 in both denormal fields, and
one that asks for nothing keeps 3 in both. The widths are not independent: fp16
operations are affected by the 32-bit field as well, so a launch MUST program both
denormal fields to the same value.

## Limits

| Limit | Value |
|-------|-------|
| Wave width | 32 lanes (ABI; not a proven maximum). |
| Local size per dimension | 1 to 1024 (10-bit field, value minus one). |
| Workgroups per dimension on the wire | 65,535. |
| Invocations per workgroup, advertised | 256. |
| Local size Z, advertised | 64. |
| Shared window per workgroup, advertised | 16,384 bytes. |
| Shared window representable in resource word 0 | 262,016 bytes. |
| LDS aperture | 4 GiB. |
| Registers | 102 SGPRs and 256 VGPRs per lane; the descriptor names at most 120 and 252. |

The advertised workgroup limits are host policy below the encoding's bound: a
launch MAY advertise less than the 1024 per dimension the local-size word can
name, and SHOULD NOT advertise more.

## Open questions

- The meaning of the three remainder fields and grid dimensions; a launch writes
  zero there and nothing consumes them.
- Whether an LDS access outside the declared window faults, reads another
  workgroup's region, or is masked.
- Whether the hardware requires an ordering instruction in front of the first
  workgroup barrier of a kernel.
- Whether a rounding mode other than nearest-even is honoured.
- The hardware's maximum invocation product per workgroup. The advertised value
  is policy; no source establishes the architectural bound.
