// load_v_buff_b96 -- Reads a 96-bit bit pattern from a descriptor-addressed buffer into a three-register vector tuple.
//
// buff-carrier load: the descriptor/address formation is part of the buffer contract; the CPU model passes the loaded words through (wider tuples by their leading words).

dst[0] = src[0];
dst[1] = src[1];
dst[2] = src[2];
