// v_rsq_f32 -- Approximates the reciprocal square root of an f32 value.
//

u32 v = src[0];
if ((v & 0x7fffffff) == 0) {
  dst[0] = (v & 0x80000000u) ? 0xff800000u : 0x7f800000u;
} else if ((v & 0x7f800000u) == 0x7f800000u && (v & 0x007fffffu) != 0u) {
  dst[0] = v;
} else if ((v & 0x80000000u) != 0u) {
  dst[0] = 0xffc00011u;
} else {
  dst[0] = f32_rsq(v, policy);
}
