// load_v_buff_i8 -- Reads one signed byte through a buffer descriptor and sign-extends it into one 32-bit vector register.
//
// buff-carrier load: the descriptor/address formation is part of the buffer contract; the CPU model passes the loaded words through (wider tuples by their leading words).

dst[0] = src[0];
