// load_v_buff_i16 -- Loads one signed halfword and sign-extends it to 32 bits from the buffer descriptor plus the encoded address and offsets into the vector destination
// [HW-observed] buff-carrier load: the descriptor/address formation is part of the buffer contract; the CPU model passes the loaded words through (wider tuples by their leading words).
dst[0] = sext_i32_i16(src[0]);
