// v_mul_legacy_imm_f32 -- Multiplies an f32 vector value by the raw binary32 literal under legacy rules.
//

// The legacy rules are the same ones v_mul_legacy_f32 carries, and this row had none of
// them: an operand denormal is flushed, an infinity times a zero gives the zero with the
// XOR of the signs rather than a NaN, and the result is flushed too.  With only the plain
// multiply the row failed 73 cases, every one of them one of these three.
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
