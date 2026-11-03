// v_add_u32 -- Adds two u32 values: `d = a + b` (mod 2^32), plus the carry-out.
//
// The carry-out is a per-lane bitmap in the low word of the VCC pair the RD8
// operand names [HW 2026-11-03], bit i = lane i's carry-out, and s7 is never
// written.  Both addends are uniform across the sixteen lanes, so every lane
// carries or none does: the bitmap is 0xffff or 0x0000, never one bit.

u32 lhs = src[0];
u32 rhs = src[1];
u32 sum = lhs + rhs;
dst[0] = sum;
dst[1] = (sum < lhs) ? 0xffffu : 0u;
