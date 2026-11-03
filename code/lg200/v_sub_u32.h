// v_sub_u32 -- Subtracts the second u32 source from the first, writing a difference and a borrow-out.
//
// The borrow-out is a per-lane bitmap in the low word of the VCC pair the RD8
// operand names [HW 2026-11-03], bit i = lane i's borrow-out, and s7 is never
// written.  Both operands are uniform across the sixteen lanes, so every lane
// borrows or none does: the bitmap is 0xffff or 0x0000, never one bit.

u32 diff = src[0] - src[1];
dst[0] = diff;
dst[1] = (src[0] < src[1]) ? 0xffffu : 0u;
