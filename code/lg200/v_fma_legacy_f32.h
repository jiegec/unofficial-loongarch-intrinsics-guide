// v_fma_legacy_f32 -- Computes a fused multiply-add in the legacy operator form: `d = a * b + c`.
//

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
if (out == 0u && ((a & 0x7fffffffu) == 0u || (b & 0x7fffffffu) == 0u) &&
    c == 0x80000000u)
  out = 0x80000000u;
dst[0] = out;
