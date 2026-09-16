// v_ldexp_f64 -- Scales the floating-point significand by two raised to the integer exponent and writes the result to the vector destination
// [HW 2026-08-24] the exponent is a signed 32-bit value; zero inputs
// stay zero, extremes clamp in sf_ldexp64 (inf / signed zero / denormal
// flush below mant 2^32).
u64 v = ((u64)src[0] | ((u64)src[1] << 32));
const std::int32_t e =
    static_cast<std::int32_t>(src[2]);
u64 r = f64_ldexp(v, static_cast<std::uint32_t>(e));
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
