// v_log2_f32 -- Computes the base-two logarithm of each lane's f32 source: `d = log2(a)`.
//

u32 v = src[0];
if ((v & 0x7fffffff) == 0) {
  dst[0] = 0xff800000u;
} else if ((v & 0x7f800000u) == 0x7f800000u && (v & 0x007fffffu) != 0u) {
  dst[0] = v;
} else if ((v & 0x80000000u) != 0u) {
  dst[0] = 0xffc00012u;
} else {
  dst[0] = f32_log2_(v, policy);
}
