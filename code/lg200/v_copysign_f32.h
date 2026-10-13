// v_copysign_f32 -- Combines the magnitude of one f32 value with the sign bit of another: `d = {sign(b), magnitude(a)}`.
//

// [HW 2026-10-13] the row flushes both operands and the result regardless of the case's
// token: copysign(0x1, 0x80000000) returns -0 rather than the denormal -0x80000001, and
// copysign(0x1, 0x0) returns +0.  sf_copysign32 is a bit operation and cannot flush, so
// the three flushes are explicit here, as the legacy arithmetic rows do.
auto ftz = [](u32 v) -> u32 {
  return (v & 0x7f800000u) == 0u ? (v & 0x80000000u) : v;
};
dst[0] = ftz(f32_copysign(ftz(src[0]), ftz(src[1]), policy));
