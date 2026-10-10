// load_v_buff_i16 -- Reads one signed halfword through a buffer descriptor and sign-extends it into one 32-bit vector register.
//
// buff-carrier load: the descriptor/address formation is part of the buffer contract; the CPU model passes the loaded words through (wider tuples by their leading words).

dst[0] = sext_i32_i16(src[0]);
