// v_fcvt_f32_f64 -- Converts the 64-bit floating-point value source to a 32-bit floating-point value and writes the result to the vector destination
// LG200 hardware flushes subnormal (denormal) f32 outputs to zero on this
// conversion path (no carrier FP mode exposes the denormal handling).
u32 r = f64_f32(((u64)src[0] | ((u64)src[0+1] << 32)));
if ((r & 0x7f800000u) == 0u && (r & 0x007fffffu) != 0u) {
  r = r & 0x80000000u;
}
dst[0] = r;
