// v_mul_hi_u32 -- Multiplies two u32 values at full width, returning the high 32 bits: `d = (a * b) >> 32`.
//

dst[0] = mul_hi_u32(src[0], src[1]);
