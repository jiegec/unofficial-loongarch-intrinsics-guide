// v_sqrt_f32 -- Computes the square root of the 32-bit floating-point source and writes the result to the vector destination
// [HW 2026-08-24] negative (non-zero, non-NaN) inputs yield the
// fixed negative-NaN 0xffc00014; -0.0 keeps -0.0; NaN propagates.
u32 v = src[0];
if ((v & 0x7fffffff) == 0u) {
  dst[0] = v;
} else if ((v & 0x7f800000u) == 0x7f800000u && (v & 0x007fffffu) != 0u) {
  dst[0] = v;
} else if ((v & 0x80000000u) != 0u) {
  dst[0] = 0xffc00014u;
} else {
  dst[0] = f32_sqrt(v, policy);
}
