// v_ldexp_f64 -- Scales each lane's f64 significand by two raised to a signed integer exponent: `d = a * 2^e`.
//

u64 v = ((u64)src[0] | ((u64)src[1] << 32));
const std::int32_t e =
    static_cast<std::int32_t>(src[2]);
u64 r = f64_ldexp(v, static_cast<std::uint32_t>(e));
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
