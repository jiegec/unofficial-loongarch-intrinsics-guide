// v_fract_f32 -- Computes the fractional part of each lane's f32 source: `d = a - floor(a)`.
//
// [HW 2026-10-22] two rules the model had wrong.  A denormal input is flushed BEFORE the
// subtraction - 0x80000001 returns 0x00000000 where the exact fraction is 1.0 - and a
// signaling NaN is QUIETED, 0x7f800001 returning 0x7fc00001 with sign and payload kept,
// where the passthrough returned it untouched.

u32 in = src[0];
in = (in & 0x7f800000u) == 0u ? (in & 0x80000000u) : in;  // a denormal flushes to signed zero
const u32 mag = in & 0x7f800000u;
if (mag == 0x7f800000u && (in & 0x007fffffu) != 0u) {
  dst[0] = in | 0x00400000u;  // a NaN operand is quieted, sign and payload kept
} else if (mag == 0x7f800000u) {
  dst[0] = 0u;
} else {
  u32 out = f32_fract(in, policy);
  if ((out & 0x7fffffff) == 0u)
    out = 0u;
  dst[0] = out;
}
