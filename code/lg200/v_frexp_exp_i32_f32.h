// v_frexp_exp_i32_f32 -- Returns the unbiased exponent of each lane's f32 source: `d = e` for `a = m * 2^e` with `m` in [0.5, 1).
//

dst[0] = frexp_exp_i32_f32(src[0]);
