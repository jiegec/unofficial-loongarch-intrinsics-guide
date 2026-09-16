// v_frexp_exp_i32_f64 -- Extracts the unbiased exponent from the 64-bit floating-point value source and writes it as a signed 32-bit integer to the vector destination for each active lane
// [HW 2026-08-24] denormal inputs flush to zero before frexp.
u64 v = ((u64)src[0] | ((u64)src[0+1] << 32));
if ((v & 0x7ff0000000000000ull) == 0 && (v & 0x000fffffffffffffull) != 0)
  v = (v & 0x8000000000000000ull);
dst[0] = frexp_exp_i32_f64(v);
