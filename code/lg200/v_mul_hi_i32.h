// v_mul_hi_i32 -- Multiplies two i32 values at full width, returning the high 32 bits: `d = (a * b) >> 32`.
//

dst[0] = mul_hi_i32(src[0], src[1]);
