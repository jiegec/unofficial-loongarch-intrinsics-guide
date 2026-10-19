// store_v_buff_fmtdesc_c1 -- Stores 1 formatted component using the resource descriptor to the buffer descriptor plus the encoded address and offsets
// [HW-observed] buff-carrier formatted store: the descriptor/address formation and the format-space encoding are part of the buffer contract (format decode UNRESOLVED); the CPU model passes the stored words through.
dst[0] = src[0];
