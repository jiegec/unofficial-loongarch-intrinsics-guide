// store_v_buff_b32 -- Writes 32 bits of one vector register to a descriptor-addressed buffer location.
//
// buff-carrier store: the descriptor/address formation is part of the buffer contract; the CPU model passes the stored words through (wider tuples by their leading words).

dst[0] = src[0];
