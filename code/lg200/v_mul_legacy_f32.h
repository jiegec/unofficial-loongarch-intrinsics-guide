// v_mul_legacy_f32 -- Multiplies two f32 values under the legacy zero-and-infinity rule: `d = a * b`.
//

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
