// v_mul24_u32 -- Multiplies the zero-extended low 24 bits of two u32 values and returns the low 32 bits: `d = (zext24(a) * zext24(b)) mod 2^32`.
//

dst[0] = mul24_u32(src[0], src[1]);
