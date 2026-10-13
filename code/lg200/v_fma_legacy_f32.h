// v_fma_legacy_f32 -- Computes a fused multiply-add in the legacy operator form: `d = a * b + c`.
//

auto ftz = [](u32 v) -> u32 {
  return (v & 0x7f800000u) == 0u ? (v & 0x80000000u) : v;
};
u32 a = ftz(src[0]), b = ftz(src[1]), c = ftz(src[2]);
u32 out;
// [HW 2026-10-13] the zero-product path is an addition, not a fused multiply-add.
// With a or b zero the hardware adds c to the product's signed zero, and a
// signaling NaN in c comes back unquieted: (0, 1, 0x7f800001) returns
// 0x7f800001 where the fused path would return 0x7fc00001.  That distinction was
// all 88 failures of this row - the modern v_mac and v_fma quiet the NaN and
// this legacy form does not, which is why the same rule could not be shared.
const bool c_is_nan =
    (c & 0x7f800000u) == 0x7f800000u && (c & 0x007fffffu) != 0u;
const u32 product_zero = (a ^ b) & 0x80000000u;
if ((a & 0x7fffffffu) == 0u || (b & 0x7fffffffu) == 0u) {
  out = c_is_nan ? c : f32_add(product_zero, c, policy);
} else {
  out = f32_fma(a, b, c, policy);
}
if ((out & 0x7f800000u) == 0u && (out & 0x007fffffu) != 0u)
  out &= 0x80000000u;
if (out == 0u && ((a & 0x7fffffffu) == 0u || (b & 0x7fffffffu) == 0u) &&
    c == 0x80000000u)
  out = 0x80000000u;
dst[0] = out;
