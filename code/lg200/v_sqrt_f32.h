// v_sqrt_f32 -- Computes the f32 square root: `d = sqrt(a)`.
//

u32 v = src[0];
// [HW 2026-10-13] a denormal operand flushes to signed zero first, so sqrt(0x1) is 0
// and sqrt(0x807fffff) is -0, where the model used to take the square root of a tiny
// normal.  The NaN passthrough below is right as it stands - this row does not quiet.
v = (v & 0x7f800000u) == 0u ? (v & 0x80000000u) : v;
if ((v & 0x7fffffff) == 0u) {
  dst[0] = v;
} else if ((v & 0x7f800000u) == 0x7f800000u && (v & 0x007fffffu) != 0u) {
  // [HW 2026-10-13] and the NaN branch DOES quiet on this row: 0x7f800001 comes back
  // 0x7fc00001.  The earlier batch patch edited the zero branch above instead, which
  // is why it made this row worse rather than better.
  dst[0] = v | 0x00400000u;
} else if ((v & 0x80000000u) != 0u) {
  dst[0] = 0xffc00014u;
} else {
  dst[0] = f32_sqrt(v, policy);
}
