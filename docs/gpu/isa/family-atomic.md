# Atomic Family

The instruction index for this family is under [atomic](../instructions/atomic.md).

Atomics read a location, compute a new value from it and write the new value
back, indivisibly with respect to other atomic operations on the same location.
The family exists in four memory spaces and in two widths.

## Spaces and forms

| Space | Mnemonic prefix | Address comes from |
|-------|-----------------|--------------------|
| Workgroup shared memory | `atomic_sm_*` | an offset operand |
| Buffer | `atomic_v_buff_*` | a buffer descriptor plus an index and an offset |
| Flat | `atomic_v_flat_*` | an absolute 64-bit address pair |
| Image | `atomic_v_tex_*` | an image descriptor plus a coordinate window |

Buffer, flat and image forms operate on 32- and 64-bit elements; the
shared-memory family covers the same operations plus a wider set.

## Operations

The shared-memory family has the full set:

| Group | Operations |
|-------|------------|
| Arithmetic | add, subtract, reverse subtract |
| Bitwise | and, or, xor |
| Counters | increment, decrement |
| Selection | minimum, maximum, in signed and unsigned variants |
| Exchange | swap, and compare-and-swap with an equality predicate |
| Ordered exchange | compare-and-swap with a floating-point ordering predicate |

The buffer, flat and image spaces expose the same arithmetic, bitwise, counter
and selection operations, but their exchange forms are equality-based
compare-and-swap; the ordered floating-point exchange and the reverse subtract
exist only in the shared-memory family.

## Encoding

The operation is selected by a contiguous field in the high word.  The field's
value enumerates the family in a fixed order: the 32-bit members first (swap,
compare-and-swap, add, subtract, signed minimum, unsigned minimum, signed
maximum, unsigned maximum, and, or, xor, increment, decrement, reverse
subtract, ordered-greater, ordered-less), then the same order again for the
64-bit members.

Width is therefore not a modifier: a 64-bit form is a different operation code,
and its operands become register pairs while the address operand stays a single
register.

## Destination contract

Every atomic writes two things back to the destination tuple:

| Slot | Contents |
|------|----------|
| First | the value read at the start of the operation |
| Second | the value written back |

A 32-bit operation therefore writes two words, and a 64-bit operation four words
with the low word first.  Because the read value is always returned, a failed
compare-and-swap still returns the previous contents, and success is detected by
comparing the returned value with the expected one.

## Counter semantics

The counter forms are bounded, and the bound is the operation's source operand
rather than an addend:

```text
increment:  new = old + 1        when old <  bound
            new = 0              otherwise
decrement:  new = old - 1        when 0 < old <= bound
            new = bound          otherwise
```

Two consequences are worth stating: the two operations are not inverses for a
value above the bound, and a bound of zero pins the counter at zero.

## Comparison semantics

* The equality form compares bit patterns, with no ordering.  A failed compare
  still performs the read-modify-write, storing the value read back unchanged,
  so the operation stays indivisible.
* The ordered floating-point forms keep one of the two values according to the
  predicate: the greater-than form keeps the smaller value and the less-than
  form keeps the larger one.
* An unordered comparison is false, so a NaN pattern survives the operation bit
  for bit.
* The ordered 64-bit floating-point forms have been observed to leave the
  element unchanged for every tested pair, including pairs where the ordering
  predicate holds.  A producer must not rely on them to perform the update; the
  cause is UNRESOLVED.
* Signedness is a property of the encoded operation, not a modifier: the signed
  and unsigned minimum and maximum are distinct operation codes and give
  different results for the same operands.

Image atomics are the exception to the whole family: only the 32-bit image swap
performs the read-modify-write.  The other image forms execute without touching
memory, so their behaviour must not be relied on.

## Concurrency

Several lanes of one instruction may target the same location.  The locations
are updated one lane at a time, so the instruction's result is an accumulation in
lane order rather than an independent value per lane.  The lane service order,
the behaviour of an inactive lane, and the ordering of atomic operations against
other instructions in the same invocation are UNRESOLVED.

## UNRESOLVED

> Items marked **settled** here have been closed; they are kept in place so the change
> is visible rather than silent.


* The unit of the shared-memory offset operand is **BYTES**, settled 2026-10-13: the sm rows carry a byte address in `VK8`, and the closed producer's own code for `l[t + 1]` on an `int` array adds `0x4` to it rather than `0x1`.
* Alignment requirements per width, and the behaviour of a misaligned access.
* Nothing: **the counter bound IS the source operand** (`VJ8`) - the row's own semantics give `new = (old < b) ? old + 1 : 0` and its seed passes one.  The earlier claim that no field carries it was wrong (corrected 2026-10-13).  What remains is narrower: no producer path exercises the counter forms, because the closed backend lowers `atomic_inc`/`atomic_dec` to add/sub with 1 - so what the hardware does AT the bound is untested.
* Nothing: the rows answer it - "The data operand is the destination register: the form is
  two-address, so the incoming value and the returned old value occupy the same register"
  (`VD8`).  Closed 2026-10-13.
* Out-of-range behaviour for a descriptor-relative access and for an image
  coordinate.
* Whether the image forms other than the 32-bit swap are permanently inert or
  merely unobserved.
* Narrowed, not closed.  The predicate field is the per-instruction mask selector and
  its semantics are settled ([Operands](operands.md#conditional-execution)); an
  established producer uses all four (p0 2772, p1 5244, p2 4290, p3 4142 in one whole
  linked program).  `bl1` and `bl2` are reachable booleans - the buffer forms' two
  parameters, ordered bl2 then bl1 - and only `bl1` is used, on private spills.  What
  remains open is what the two memory behaviour bits MEAN.
* Nothing: **they do not**.  U2 established it (producer static + graphics measurement,
  2026-10-12): the flat, atomic and shared forms do not clear the lane mask.  Closed
  2026-10-13 by reference.
* Whether a 64-bit update is one 64-bit access or two 32-bit steps.
* The cause of the ordered 64-bit floating-point forms not updating is narrowed, not
  open: those forms have only two 64-bit operands in the layout and no `VA8` field, so
  the expected value must be **two-address** - it comes from the destination register,
  as the buffer atomics' data operand does.  A carrier that placed the expected value in
  a register of its own would compare against whatever the destination happened to hold,
  which would fail on nearly every pair and leave the element unchanged: exactly what was
  measured.  The row states this; what is still unproven is that this is the whole story.
