// store_v_buff_b96 -- Writes three consecutive 32-bit words to a descriptor-addressed buffer location.
//
// buff-carrier store: the descriptor/address formation is part of the buffer contract; the CPU model passes the stored words through (wider tuples by their leading words).

dst[0] = src[0];
dst[1] = src[1];
dst[2] = src[2];
