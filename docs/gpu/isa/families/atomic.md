# Atomic Family

An atomic reads a memory element, computes a new value and writes it back as one
indivisible action against other atomics on that element. The family has 97 rows
in four spaces; this chapter fixes the spaces, the codes, what the destination
receives and the ordering a producer must establish. The rows are in the
[atomic index](../../instructions/atomic.md).

## Spaces

| Space | Prefix | Rows | Address | Data | Destination |
|-------|--------|-----:|---------|------|-------------|
| Shared memory | `atomic_sm_` | 32 | `VJ8`, `INST_LO[23:16]` | `VK8`, `INST_LO[7:0]` | `VD8`, `INST_LO[31:24]` |
| Buffer | `atomic_v_buff_` | 26 | descriptor, scalar offset, `UO12` | the destination group | `VD8`, `INST_LO[31:24]` |
| Flat | `atomic_v_flat_` | 26 | `VJ8x2`, `INST_HI[7:0]` | `VA8`, `INST_LO[7:0]` | `VD8`, `INST_HI[15:8]` |
| Image | `atomic_v_tex_` | 13 | descriptor group and coordinate group | `VJ8x8` | `VD8`, `INST_LO[31:24]` |

No base register is encoded for a shared access: the launch supplies the segment
and `VJ8` is the per-lane byte offset. The shared compare-and-exchange rows move
the fields — `VA8` carries the expected value, `VK8` the replacement, `VJ8` the
offset.

A buffer element is addressed by the four-scalar descriptor `RJ8x4`, the scalar
offset `RK8`, the immediate `UO12` and the `offen`, `idxen`, `bl1` and `bl2`
modifiers, as the [memory family](memory.md) defines for buffer accesses. `VQ8`
holds the constant-zero selector, not a register.

A flat element is the absolute 64-bit address in the even-aligned pair `VJ8x2`;
`RK8x2` is a zero scalar pair, not an operand. An image row takes eight-register
descriptor and source groups, and its value comes from the source group, not from
the destination register.

## Operations

| Operation | Suffixes | Update | Spaces |
|-----------|----------|--------|--------|
| Exchange | `swap_b32`, `swap_b64` | `new = src` | all four |
| Compare and exchange | `cmpswap_eq_u32`, `cmpswap_eq_u64`, `cmpswap_b32`, `cmpswap_b64` | `new = (old == cmp) ? repl : old` | all four |
| Add | `add_u32`, `add_u64`, `add_i32`, `add_i64` | `new = old + src`, modulo the width | all four |
| Subtract | `sub_u32`, `sub_u64`, `sub_i32`, `sub_i64` | `new = old - src`, modulo the width | all four |
| Reverse subtract | `rsub_u32`, `rsub_u64` | `new = src - old`, modulo the width | shared memory |
| Minimum | `min_i32`, `min_i64`, `min_u32`, `min_u64` | the smaller, by the code's signedness | all four |
| Maximum | `max_i32`, `max_i64`, `max_u32`, `max_u64` | the larger, by the code's signedness | all four |
| And, or, xor | `and_b32` … `xor_b64` | bitwise over the whole element | all four |
| Increment | `inc_u32`, `inc_u64` | bounded count up, below | all four |
| Decrement | `dec_u32`, `dec_u64` | bounded count down, below | all four |
| Ordered exchange, greater | `cmpswap_gt_f32`, `cmpswap_gt_f64` | `new = (old > src) ? src : old` | shared memory |
| Ordered exchange, less | `cmpswap_lt_f32`, `cmpswap_lt_f64` | `new = (old < src) ? src : old` | shared memory |

The shared space spells add and subtract `_u32` and `_u64`, the other three
`_i32` and `_i64`; both are one modular operation. Signedness changes the result
only for minimum and maximum, which carry a signed and an unsigned code in every
space rather than a modifier.

## Encoding

The code is `INST_HI[26:18]` in the shared-memory and tex-buffer formats and
`INST_HI[27:18]` in the flat format ([Encoding](../encoding.md)). Width is not a
modifier: a 64-bit row has its own code and even-aligned pairs, and only the
shared address stays one register. Codes are consecutive within a space and
follow the table above, a 64-bit block repeating it; the shared space ends each
block with reverse subtract and the two ordered forms.

| Space | 32-bit codes | 64-bit codes |
|-------|--------------|--------------|
| Shared memory | `0x00`–`0x0f` | `0x10`–`0x1f` |
| Buffer | `0x51`–`0x5d` | `0x5e`–`0x6a` |
| Flat | `0x00`–`0x0c` | `0x0d`–`0x19` |
| Image | `0x3a`–`0x46` | none |

`PJ2` at `INST_HI[17:16]` selects the lane mask on every atomic row.

## Destination

Every row writes the value read before the update to its destination: one VGPR
for a 32-bit row, an even-aligned pair for a 64-bit row, low word first. The
updated value stays in memory; no register receives it, and no operand reports
the outcome. There is no discard form — the register is named and overwritten
even when the old value is unwanted.

Buffer rows consume the destination as the source too, so a live value MUST be
copied out first; the buffer compare-and-exchange consumes two registers in the
32-bit row and four in the 64-bit one. Shared and flat rows name the data in a
field of its own, so a producer MAY overlap the two, and the row reads its
operands before writing the result.

## Counters

The bound is the data operand, the value the row would otherwise add or subtract:

```text
increment: new = (old < bound) ? old + 1 : 0
decrement: new = (old > 0 && old <= bound) ? old - 1 : bound
```

The cell cycles through `0` to `bound` inclusive, so the two are exact inverses
inside the range. A value outside it is lost: above the bound, one increment
takes the element to zero and one decrement to the bound. A bound of zero pins
the element at zero both ways.

## Comparison

Equality is over the whole element, bit for bit. A failed compare still performs
the read-modify-write and stores the value just read, so a lane that loses a race
still receives the pre-update content and detects the loss by comparing it.

The ordered forms use the source as the replacement, so a lane cannot compare
against a third value: `cmpswap_gt_f32` keeps the smaller of the two,
`cmpswap_lt_f32` the larger. Comparison reads bit patterns, so the two zeros
compare equal, and an unordered comparison is false: a NaN on either side
suppresses the update and both patterns survive.

The ordered 64-bit forms do not update the element — every pair leaves it as read,
including pairs where the predicate holds. A producer MUST NOT use them to update
an element. Whether the failure belongs to the width or to the code is
UNRESOLVED, so the 32-bit forms MUST NOT be assumed to share it.

Of the thirteen image rows only `atomic_v_tex_swap_b32` performs the
read-modify-write; the other twelve leave the element untouched, and whether they
write their destination is UNRESOLVED.

## Concurrency

An atomic is per-lane: `PJ2` selects the lane mask, and a lane whose bit is clear
performs no access. What such a lane leaves in its destination is UNRESOLVED.

Several lanes may name one element, each applying the operation once, so an
element named by k executing lanes is updated k times. Order does not matter for
the commutative operations; for compare-and-exchange, reverse subtract and the
ordered forms it does. The service order and the winning lane are UNRESOLVED, so
a producer MUST NOT rely on them, nor on the value a colliding lane returns.

An atomic is outstanding work in a domain: buffer and flat rows belong to the
vector-memory domain, shared rows to the shared-memory domain, and a later access
observes the update only after a `check` naming that domain
([Control and ordering](control.md#ordering)); `check` is not a fence and does
not signal completion. Across a workgroup, shared memory also needs a `barrier`
([Barriers](control.md#barriers)). Which domain drains an image atomic is
UNRESOLVED.

## Open questions

- The alignment an element requires, and what a misaligned offset does.
- What an access outside a descriptor range, or an unmapped flat address, does.
- Whether a 64-bit update is one access or two 32-bit steps.
