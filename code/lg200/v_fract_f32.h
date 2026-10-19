// v_fract_f32 -- Computes the fractional part of the 32-bit floating-point source as source minus floor(source) and writes the result to the vector destination
// [HW 2026-08-24] fract(inf)/fract(-inf) return +0.0 (not NaN);
// fract(NaN) propagates the NaN; fract(-0.0) keeps +0.0.
const u32 mag = src[0] & 0x7f800000u;
if (mag == 0x7f800000u && (src[0] & 0x007fffffu) != 0u) {
  dst[0] = src[0];  // NaN passthrough
} else if (mag == 0x7f800000u) {
  dst[0] = 0u;
} else {
  u32 out = f32_fract(src[0], policy);
  if ((out & 0x7fffffff) == 0u)
    out = 0u;
  dst[0] = out;
}
