// v_mul_u32 -- Multiplies two u32 values and writes the low 32 bits of the product: `d = (a * b) mod 2^32`.
//

dst[0] = mul_u32(src[0], src[1]);
