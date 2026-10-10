// load_v_buff_b32 -- Reads a 32-bit bit pattern from a descriptor-addressed buffer into one vector register.
//
// buff-carrier load: the descriptor/address formation is part of the buffer contract; the CPU model passes the loaded words through (wider tuples by their leading words).

dst[0] = src[0];
