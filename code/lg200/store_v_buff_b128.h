// store_v_buff_b128 -- Writes a four-register vector tuple to a descriptor-addressed buffer location.
//
// buff-carrier store: the descriptor/address formation is part of the buffer contract; the CPU model passes the stored words through (wider tuples by their leading words).

dst[0] = src[0];
dst[1] = src[1];
dst[2] = src[2];
dst[3] = src[3];
