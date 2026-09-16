// v_dp4a_u8_u8_i32 -- Computes the dot product of four packed unsigned 8-bit integer and unsigned 8-bit integer elements, adds the signed 32-bit integer accumulator, and writes the low 32-bit result to the vector destination for each active lane
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = dp4a_u8_u8_i32(src[0], src[1]);
