// v_rcp_f32 -- Computes the reciprocal of the 32-bit floating-point source and writes the result to the vector destination
// [HW 2026-08-24] denormal results flush to +0 (rcp(max)) and
// denormal inputs flush too.
u32 v = (src[0] & 0x7f800000u) == 0u ? (src[0] & 0x80000000u) : src[0];
u32 out = f32_rcp(v, policy);
if ((out & 0x7f800000u) == 0u && (out & 0x007fffffu) != 0u)
  out &= 0x80000000u;
dst[0] = out;
