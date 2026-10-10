// store_v_buff_fmtdesc_c3 -- Converts three components into the resource-declared element format and stores them.
//
// buff-carrier formatted store: the descriptor/address formation and the format-space encoding are part of the buffer contract (format decode UNRESOLVED); the CPU model passes the stored words through.

u64 r = ((u64)src[0] | ((u64)src[0+1] << 32));
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
