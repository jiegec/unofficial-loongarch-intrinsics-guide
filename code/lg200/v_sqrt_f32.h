// v_sqrt_f32 -- Computes the f32 square root: `d = sqrt(a)`.
//

u32 v = src[0];
if ((v & 0x7fffffff) == 0u) {
  dst[0] = v;
} else if ((v & 0x7f800000u) == 0x7f800000u && (v & 0x007fffffu) != 0u) {
  dst[0] = v;
} else if ((v & 0x80000000u) != 0u) {
  dst[0] = 0xffc00014u;
} else {
  dst[0] = f32_sqrt(v, policy);
}
