// v_mul24_hi_u32 -- Multiplies the zero-extended low 24 bits of two u32 values and returns the high 32 bits: `d = (zext24(a) * zext24(b)) >> 32`.
//

dst[0] = mul24_hi_u32(src[0], src[1]);
