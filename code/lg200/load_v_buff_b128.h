// load_v_buff_b128 -- Loads a 128-bit bit pattern from the buffer descriptor plus the encoded address and offsets into the vector destination
// [HW-observed] buff-carrier load: the descriptor/address formation is part of the buffer contract; the CPU model passes the loaded words through (wider tuples by their leading words).
dst[0] = src[0];
dst[1] = src[1];
dst[2] = src[2];
dst[3] = src[3];
