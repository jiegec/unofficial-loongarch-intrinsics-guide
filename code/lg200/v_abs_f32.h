// v_abs_f32 -- Clears the sign bit of an f32 value: `d = a & 0x7fffffff`.
//

dst[0] = src[0] & 0x7fffffffu;
