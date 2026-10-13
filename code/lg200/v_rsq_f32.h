// v_rsq_f32 -- Approximates the reciprocal square root of an f32 value.
//

u32 v = src[0];
// [HW 2026-10-13] two corrections measured on this row alone.  A denormal operand is
// flushed to signed zero before the branches, so rsq(0x1) returns +inf and
// rsq(0x80000001) -inf, where the model used to divide by a tiny normal.  And the
// machine QUIETS a NaN operand, 0x7f800001 coming back 0x7fc00001, where the model
// passed it through untouched.
v = (v & 0x7f800000u) == 0u ? (v & 0x80000000u) : v;
if ((v & 0x7fffffff) == 0) {
  dst[0] = (v & 0x80000000u) ? 0xff800000u : 0x7f800000u;
} else if ((v & 0x7f800000u) == 0x7f800000u && (v & 0x007fffffu) != 0u) {
  dst[0] = v | 0x00400000u;
} else if ((v & 0x80000000u) != 0u) {
  dst[0] = 0xffc00011u;
} else {
  dst[0] = f32_rsq(v, policy);
}
