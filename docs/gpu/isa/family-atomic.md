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

* The unit of the shared-memory offset operand: byte or element.
* Alignment requirements per width, and the behaviour of a misaligned access.
* Where the counter bound is read from, since no encoded register field carries
  it in the counter forms.
* Which register supplies the data operand of the 32-bit buffer forms.
* Out-of-range behaviour for a descriptor-relative access and for an image
  coordinate.
* Whether the image forms other than the 32-bit swap are permanently inert or
  merely unobserved.
* The meaning of the predicate field, and of the two memory behaviour controls.
* Whether the atomic forms affect lane masks.
* Whether a 64-bit update is one 64-bit access or two 32-bit steps.
* The cause of the ordered 64-bit floating-point forms not updating.
