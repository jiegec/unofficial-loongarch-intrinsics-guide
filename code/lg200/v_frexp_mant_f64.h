// v_frexp_mant_f64 -- Extracts the normalized mantissa from the 64-bit floating-point value source and writes it to the vector destination for each active lane
// [HW 2026-08-24] denormal inputs flush to zero before frexp.
u64 v = ((u64)src[0] | ((u64)src[0+1] << 32));
if ((v & 0x7ff0000000000000ull) == 0 && (v & 0x000fffffffffffffull) != 0)
  v = (v & 0x8000000000000000ull);
u64 r = frexp_mant_f64(v);
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
