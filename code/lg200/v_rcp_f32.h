// v_rcp_f32 -- Produces an approximation of the reciprocal of an f32 value: `d = 1/a`.
//

u32 v = (src[0] & 0x7f800000u) == 0u ? (src[0] & 0x80000000u) : src[0];
u32 out = f32_rcp(v, policy);
if ((out & 0x7f800000u) == 0u && (out & 0x007fffffu) != 0u)
  out &= 0x80000000u;
dst[0] = out;
