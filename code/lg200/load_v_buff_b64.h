// load_v_buff_b64 -- Reads a 64-bit bit pattern from a descriptor-addressed buffer into an aligned pair of vector registers.
//
// buff-carrier load: the descriptor/address formation is part of the buffer contract; the CPU model passes the loaded words through (wider tuples by their leading words).

dst[0] = src[0];
dst[1] = src[1];
