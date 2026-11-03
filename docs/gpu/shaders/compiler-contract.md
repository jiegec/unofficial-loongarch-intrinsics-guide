# Compiler Contract

This chapter collects the hardware constraints that a compiler must respect when
it selects, schedules and packs instructions. Some rules are architectural; some
are conventions of the current compiler. A **Host policy** item is not a hardware
requirement.

## Condition register

The condition register `SCC` is one bit per wave. Compare instructions write it;
add-with-carry, subtract-with-borrow, select and conditional branches read it.

- A consumer of `SCC` MUST be materialised before the next compare is issued.
  Back-to-back compares can read a stale value.
- A compare that writes a scalar register rather than `SCC` is not the
  established form.
- A carry or borrow is encoded as "the condition is non-zero"; a producer MUST
  treat it as a boolean.

## Lane masks

- `p0..p3` hold absolute lane sets. An instruction selects one with the 2-bit
  mask field.
- A gated write suppresses the write; it does not clear the lane.
- `p1..p3` are undefined at launch. A shader MUST write a mask before selecting
  it.
- A region is entered by narrowing the current mask and left by restoring the
  saved mask.
- A compare can write a mask register while the instruction is gated by that
  same register. The selector is read before the write, so the result is
  `old_mask & condition`.

## Ordering instruction

The ordering instruction drains the pending work named by its four domain bits:

| Bit | Domain |
|----:|--------|
| 0 | Kernel and scalar. |
| 1 | Vector memory. |
| 2 | Vector store. |
| 3 | Shared memory. |

The bits are independent and may be combined. A compiler MUST place an ordering
instruction:

- after a memory or sampling operation and before its result is used;
- after a descriptor fetch and before the descriptor is read;
- after an interface write and before the next write reuses the register.

The ordering instruction is not a barrier and not a host fence. It may be
placed inside a narrowed region, and it orders that region's work; an ordering
instruction outside the region does not order work inside it.

## Descriptor staging

A lane-mask register is not the only implicit input. An SIO write reads the
record descriptor from the register whose number equals the channel slot. A
producer MUST stage that descriptor with a register move before the write.

A texture instruction's image descriptor occupies eight SGPRs; its sampler
descriptor occupies four. The tuples MUST be aligned as described in
[Descriptors](descriptors.md#alignment). The destination VGPR MUST be four-aligned
and the channel mask MUST be nonzero.

## Tuples

A tuple base MUST be aligned to its width. A two-register tuple starts at an even
register, a four-register tuple at a multiple of four, an eight-register tuple at
a multiple of eight.

> UNRESOLVED: whether an odd-base pair is legal. Usage evidence shows only even
> bases, but the allocator may simply never produce an odd one.

## Memory hazards

- A load followed by a use of its result needs an ordering instruction in
  between.
- A store followed by a load of the same address needs an ordering instruction
  between them.
- Two writes to the same SIO channel need an ordering instruction between them.
- An atomic's result is not visible to a later ordinary load without an ordering
  instruction.

## Floating point

The graphics pipe flushes denormal inputs and results and rounds to nearest. The
rounding-direction and flush controls in an instruction's encoding are inert on
the graphics pipe.

The compute pipe takes its rounding and denormal mode from the kernel
descriptor's resource word. A compute shader that requires a non-default mode
gets it from the dispatch, not from the instruction.

A NaN produced by an operation is the positive quiet pattern `0x7fc00000`; a
propagated NaN keeps the source sign and payload. Minimum and maximum propagate
NaN.

## Entry and exit

- A stage captures the active lane mask at entry and restores it at a
  non-terminating exit.
- A stage that uses helper lanes installs a whole-quad mask after the capture.
- A stage writes its outputs, commits the export, orders and terminates.
- A discard restores the mask, clears the lanes that must not write and
  continues.

## Host policy

The current compiler:

- reserves the low twelve SGPRs for the launch window;
- uses `v33`, `v35`, `v19` and `v45` for geometry window state;
- stages the constant root in `s4:s5` and does not use it as scratch;
- orders SIO reads in ascending channel order and copies each four-channel read
  out of its fixed destination window.

These are conventions; another compiler may choose differently as long as it
respects the hardware rules above.

## Open questions

- Whether the lane-mask window and the descriptor scratch alias.
- Whether a compare's mask write and its use in the same instruction are ordered
  as described for every family.
- The exact hazard rules for texture and shared-memory instructions.
