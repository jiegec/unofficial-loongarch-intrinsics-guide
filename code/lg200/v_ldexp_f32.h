// v_ldexp_f32 -- Scales each lane's f32 significand by two raised to a signed integer exponent: `d = a * 2^e`.
//

dst[0] = f32_ldexp(src[0], src[1], policy);
