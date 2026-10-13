// v_log2_f32 -- Computes the base-two logarithm of each lane's f32 source: `d = log2(a)`.
//

u32 v = src[0];
// [HW 2026-10-13] the same two corrections as v_rsq_f32, measured on this row: a
// denormal operand flushes to signed zero first, so log2(0x1) returns -inf, and a NaN
// operand is QUIETED rather than passed through.
v = (v & 0x7f800000u) == 0u ? (v & 0x80000000u) : v;
if ((v & 0x7fffffff) == 0) {
  dst[0] = 0xff800000u;
} else if ((v & 0x7f800000u) == 0x7f800000u && (v & 0x007fffffu) != 0u) {
  dst[0] = v | 0x00400000u;
} else if ((v & 0x80000000u) != 0u) {
  dst[0] = 0xffc00012u;
} else {
  dst[0] = f32_log2_(v, policy);
}
