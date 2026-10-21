// v_abs_f32 -- Clears the sign bit of an f32 value: `d = a & 0x7fffffff`.
//
// [HW 2026-10-22] a denormal input comes back zero even under a bare token, so the flush is
// unconditional here: 0x1 returns 0x00000000 and 0x80000001 also returns 0x00000000, where
// passing the bits through would have returned 0x007fffff and 0x00000001.

u32 v = src[0];
v = (v & 0x7f800000u) == 0u ? (v & 0x80000000u) : v;  // a denormal flushes to signed zero
dst[0] = v & 0x7fffffffu;
