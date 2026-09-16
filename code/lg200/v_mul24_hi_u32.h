// v_mul24_hi_u32 -- Multiplies the low 24 bits of the two unsigned 32-bit integer sources and writes the high result bits to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = mul24_hi_u32(src[0], src[1]);
