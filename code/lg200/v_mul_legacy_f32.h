// v_mul_legacy_f32 -- Multiplies the two floating-point sources using the legacy zero and NaN rules and writes the result to the vector destination
// [HW 2026-08-24] legacy multiply: 0*inf = 0 (no NaN trap), denormal
// inputs/results flush to signed zero.
auto ftz = [](u32 v) -> u32 {
  return (v & 0x7f800000u) == 0u ? (v & 0x80000000u) : v;
};
u32 a = ftz(src[0]), b = ftz(src[1]);
u32 out;
if (((a & 0x7fffffff) == 0u && (b & 0x7f800000u) == 0x7f800000u) ||
    ((b & 0x7fffffff) == 0u && (a & 0x7f800000u) == 0x7f800000u)) {
  out = (a ^ b) & 0x80000000u;
} else {
  out = f32_mul(a, b, policy);
}
if ((out & 0x7f800000u) == 0u && (out & 0x007fffffu) != 0u)
  out &= 0x80000000u;
dst[0] = out;
