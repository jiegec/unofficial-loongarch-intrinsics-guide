// v_fma_f32 -- Computes a fused multiply-add of the three sources with one final rounding and writes the result to the vector destination
// [HW 2026-08-24] The FMA pipe flushes denormal INPUTS to signed
// zero (maxf*denormal = 0, inf*denormal = NaN) and denormal RESULTS to
// a signed zero.
auto ftz = [](u32 v) -> u32 {
  return (v & 0x7f800000u) == 0u ? (v & 0x80000000u) : v;
};
u32 a = ftz(src[0]), b = ftz(src[1]), c = ftz(src[2]);
u32 out = f32_fma(a, b, c, policy);
if ((out & 0x7f800000u) == 0u && (out & 0x007fffffu) != 0u)
  out &= 0x80000000u;
dst[0] = out;
