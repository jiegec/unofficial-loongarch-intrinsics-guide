// v_fcvt_f64_f32 -- Converts the 32-bit floating-point value source to a 64-bit floating-point value and writes the result to the vector destination
// LG200 hardware flushes subnormal (denormal) f32 inputs to zero on this
// conversion path (no carrier FP mode exposes the denormal handling).
u32 a = src[0];
if ((a & 0x7f800000u) == 0u && (a & 0x007fffffu) != 0u) {
  a = a & 0x80000000u;
}
u64 r = f32_f64(a);
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
