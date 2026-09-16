// v_fcvt_f32off_i4 -- Converts the sign-extended low 4 bits of the source scaled by 2^-4 to 32-bit floating point and writes the result to the vector destination
// [HW 2026-08-25] the format offset takes the SIGN-EXTENDED low 4 bits of
// the source scaled by 2^-4: 0x1 -> +2^-4, 0x2 -> +2^-3, 0xe -> -2^-3,
// 0xf -> -2^-4; the upper 28 bits are ignored (verified on 14 probe
// values incl. 0x7fffffff / 0x8000000f / 0x3f800000).
dst[0] = fcvt_f32off_i4(src[0]);