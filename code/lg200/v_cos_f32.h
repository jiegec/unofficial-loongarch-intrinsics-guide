// v_cos_f32 -- Computes the cosine of an f32 value: `d = cos(a)`.
//
// [HW 2026-10-22] a NaN operand is QUIETED here as it is in sin, keeping its sign and payload:
// 0x7f800001 returns 0x7fc00001 and 0xff800001 returns 0xffc00001.

u32 v = src[0];
dst[0] = ((v & 0x7f800000u) == 0x7f800000u && (v & 0x007fffffu) != 0u)
             ? (v | 0x00400000u)
             : f32_cos(v, policy);
