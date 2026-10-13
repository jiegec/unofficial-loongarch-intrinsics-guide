// v_copysign_imm_f32 -- Copies the sign of the literal onto an f32 magnitude: `d = copysign(a, imm)`.
//

// [HW 2026-10-13] the same flushes as v_copysign_f32: both operands and the result, with
// the source 0x00000001 coming back 0x00000000 and 0x007fffff coming back 0x00000000,
// under a plain rtne token.
auto ftz = [](u32 v) -> u32 {
  return (v & 0x7f800000u) == 0u ? (v & 0x80000000u) : v;
};
dst[0] = ftz(copysign_imm_f32(ftz(src[0]), ftz(src[1]), policy));
