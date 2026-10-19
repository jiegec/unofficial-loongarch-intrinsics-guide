// store_v_buff_b32 -- Stores the low 32-bit bit pattern from the vector data source to the buffer descriptor plus the encoded address and offsets
// [HW-observed] buff-carrier store: the descriptor/address formation is part of the buffer contract; the CPU model passes the stored words through (wider tuples by their leading words).
dst[0] = src[0];
