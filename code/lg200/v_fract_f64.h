// v_fract_f64 -- Computes the fractional part of the 64-bit floating-point source as source minus floor(source) and writes the result to the vector destination
// [HW 2026-08-24] Denormal inputs flush to a signed zero before fract
// (fract(denormal) = 0 on hardware, not the denormal/1-frac forms).
u64 v = ((u64)src[0] | ((u64)src[0+1] << 32));
if ((v & 0x7ff0000000000000ull) == 0 && (v & 0x000fffffffffffffull) != 0)
  v = (v & 0x8000000000000000ull);
u64 r = f64_fract(v, policy);
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
