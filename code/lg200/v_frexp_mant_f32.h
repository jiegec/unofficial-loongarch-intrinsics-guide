// v_frexp_mant_f32 -- Returns the normalized significand of each lane's f32 source: `d = m` with `m` in [0.5, 1).
//

dst[0] = frexp_mant_f32(src[0]);
