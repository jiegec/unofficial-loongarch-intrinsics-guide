// v_fma_legacy_f32 -- Computes the legacy fused multiply-add form of the three sources and writes the result to the vector destination
// [HW 2026-08-24] Legacy FMA uses the non-standard multiply: 0*inf = 0
// (no NaN trap), denormal inputs/results flush to signed zero.
auto ftz = [](u32 v) -> u32 {
  return (v & 0x7f800000u) == 0u ? (v & 0x80000000u) : v;
};
u32 a = ftz(src[0]), b = ftz(src[1]), c = ftz(src[2]);
u32 out;
if (((a & 0x7fffffff) == 0u && (b & 0x7f800000u) == 0x7f800000u) ||
    ((b & 0x7fffffff) == 0u && (a & 0x7f800000u) == 0x7f800000u)) {
  out = f32_add(0u, c, policy);
} else {
  out = f32_fma(a, b, c, policy);
}
if ((out & 0x7f800000u) == 0u && (out & 0x007fffffu) != 0u)
  out &= 0x80000000u;
// [HW 2026-08-24] a -0.0 addend keeps the negative zero when the sum
// is exactly zero (0 + -0 = -0, and x*1 + -0 keeps -0 when x is 0).  The
// zero product counts either sign: fma(-0, -0, -0) = -0 [HW 2026-08-25].
if (out == 0u && ((a & 0x7fffffffu) == 0u || (b & 0x7fffffffu) == 0u) &&
    c == 0x80000000u)
  out = 0x80000000u;
dst[0] = out;
