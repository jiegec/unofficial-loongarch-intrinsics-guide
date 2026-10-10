// v_fcvt_f64_f32 -- Widens each lane's single-precision source to double precision: `d = f64(f32(a))`.
//

u32 a = src[0];
if ((a & 0x7f800000u) == 0u && (a & 0x007fffffu) != 0u) {
  a = a & 0x80000000u;
}
u64 r = f32_f64(a);
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
