// v_mul24_u32 -- Multiplies the low 24 bits of the two unsigned 32-bit integer sources and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = mul24_u32(src[0], src[1]);
