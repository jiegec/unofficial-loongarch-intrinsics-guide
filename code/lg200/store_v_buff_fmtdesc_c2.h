// store_v_buff_fmtdesc_c2 -- Stores 2 formatted components using the resource descriptor to the buffer descriptor plus the encoded address and offsets
// [HW-observed] buff-carrier formatted store: the descriptor/address formation and the format-space encoding are part of the buffer contract (format decode UNRESOLVED); the CPU model passes the stored words through.
u64 r = ((u64)src[0] | ((u64)src[0+1] << 32));
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
