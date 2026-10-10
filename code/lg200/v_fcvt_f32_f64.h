// v_fcvt_f32_f64 -- Narrows each lane's double-precision source to single precision: `d = f32(f64(a))`.
//

u32 r = f64_f32(((u64)src[0] | ((u64)src[0+1] << 32)));
if ((r & 0x7f800000u) == 0u && (r & 0x007fffffu) != 0u) {
  r = r & 0x80000000u;
}
dst[0] = r;
