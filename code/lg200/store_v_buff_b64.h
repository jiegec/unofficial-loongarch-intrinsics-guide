// store_v_buff_b64 -- Writes the 64 bits held in two consecutive vector registers to a descriptor-addressed buffer location.
//
// buff-carrier store: the descriptor/address formation is part of the buffer contract; the CPU model passes the stored words through (wider tuples by their leading words).

dst[0] = src[0];
dst[1] = src[1];
