// load_v_buff_fmtinst_c1 -- Loads 1 formatted component using the instruction format from the buffer descriptor plus the encoded address and offsets into the vector destination
// [HW-observed] buff-carrier formatted load: the descriptor/address formation and the format-space readback are part of the buffer contract (format decode UNRESOLVED); the CPU model passes the loaded words through.
dst[0] = src[0];
