// v_mac_f32 -- Multiplies the two 32-bit floating-point sources and adds the current destination accumulator and writes the result to the vector destination
// [HW 2026-08-24] FMA-family: denormal inputs/results flush to a
// signed zero on the tested carrier.
auto ftz = [](u32 v) -> u32 {
  return (v & 0x7f800000u) == 0u ? (v & 0x80000000u) : v;
};
u32 out = f32_fma(ftz(src[0]), ftz(src[1]), ftz(src[2]), policy);
if ((out & 0x7f800000u) == 0u && (out & 0x007fffffu) != 0u)
  out &= 0x80000000u;
dst[0] = out;
