// v_ldexp_imm_f32 -- Scales an f32 value by two raised to an exponent in the literal: `d = a * 2^e`.
//

dst[0] = f32_ldexp(src[0], src[1], policy);
