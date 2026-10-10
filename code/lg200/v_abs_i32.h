// v_abs_i32 -- Computes the absolute value of an i32: `d = |a|`.
//

dst[0] = (src[0] & 0x80000000u) == 0 ? src[0] : 0u - src[0];
