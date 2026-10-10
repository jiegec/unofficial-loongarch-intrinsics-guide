// v_mul24_i32 -- Multiplies the sign-extended low 24 bits of two i32 values and returns the low 32 bits: `d = (sext24(a) * sext24(b)) mod 2^32`.
//

dst[0] = mul24_i32(src[0], src[1]);
