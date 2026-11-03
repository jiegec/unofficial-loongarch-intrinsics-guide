# Atomic Family

Atomics read a location, compute a new value and write it back, indivisibly with
respect to other atomics on the same location. The family exists in four memory
spaces. This chapter defines the spaces, the operations and the destination
contract. The rows are in the [atomic index](../../instructions/atomic.md).

## Spaces

| Space | Prefix | Address |
|-------|--------|---------|
| Shared memory | `atomic_sm_*` | Offset within the LDS aperture. |
| Buffer | `atomic_v_buff_*` | Buffer descriptor plus index and offset. |
| Flat | `atomic_v_flat_*` | Absolute 64-bit address pair. |
| Image | `atomic_v_tex_*` | Image descriptor plus coordinate window. |

Buffer, flat and image forms operate on 32- and 64-bit elements. The shared
family covers the same operations plus a wider set.

## Operations

| Group | Operations |
|-------|------------|
| Arithmetic | Add, subtract, reverse subtract. |
| Bitwise | And, or, xor. |
| Counters | Increment, decrement. |
| Selection | Minimum and maximum, signed and unsigned. |
| Exchange | Swap, compare-and-swap with equality. |
| Ordered exchange | Compare-and-swap with a floating-point ordering predicate. |

The buffer, flat and image spaces expose the arithmetic, bitwise, counter and
selection operations; their exchange forms are equality-based. The ordered
floating-point exchange and reverse subtract exist only in the shared family.

## Encoding

The operation is selected by a contiguous field in the high word. The field
enumerates the family in a fixed order: the 32-bit members first (swap,
compare-and-swap, add, subtract, signed minimum, unsigned minimum, signed
maximum, unsigned maximum, and, or, xor, increment, decrement, reverse subtract,
ordered-greater, ordered-less), then the same order for the 64-bit members.

Width is not a modifier: a 64-bit form is a different operation code, and its
operands become register pairs while the address stays a single register.

## Destination

Every atomic writes two things to the destination tuple:

| Slot | Contents |
|------|----------|
| First | The value read at the start. |
| Second | The value written back. |

A 32-bit operation writes two words; a 64-bit operation writes four words with
the low word first. A failed compare-and-swap still returns the previous value,
so success is detected by comparing the returned value with the expected one.

## Counters

The counter forms are bounded, and the bound is the operation's source operand:

```text
increment: new = old + 1, when old < bound; otherwise 0
decrement: new = old - 1, when 0 < old <= bound; otherwise bound
```

The operations are not inverses above the bound. A bound of zero pins the
counter at zero.

## Comparison

- The equality form compares bit patterns. A failed compare still performs the
  read-modify-write, storing the value read back unchanged.
- The ordered floating-point forms keep one value according to the predicate: the
  greater-than form keeps the smaller value, and the less-than form keeps the
  larger one.
- An unordered comparison is false, so a NaN pattern survives bit for bit.
- The ordered 64-bit floating-point forms leave the element unchanged for every
  documented pair, including pairs where the predicate holds. A producer MUST
  NOT rely on them to perform the update; the cause is UNRESOLVED.

Signedness is part of the operation code, not a modifier. Signed and unsigned
minimum and maximum are distinct operations.

Image atomics are the exception: only the 32-bit image swap performs the
read-modify-write. The other image forms execute without touching memory.

## Concurrency

Several lanes of one instruction may target the same location. The locations
are updated one lane at a time, so the instruction's result is an accumulation
in lane order rather than an independent value per lane.

> UNRESOLVED: the lane service order, the behaviour of an inactive lane, and the
> ordering of an atomic against other instructions in the same invocation.

## Open questions

- Alignment requirements per width.
- Out-of-range behaviour for a descriptor-relative or image access.
- Whether the image forms other than the 32-bit swap are permanently inert.
- Whether a 64-bit update is one 64-bit access or two 32-bit steps.
