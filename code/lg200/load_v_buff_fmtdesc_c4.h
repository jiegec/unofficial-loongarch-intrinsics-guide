// load_v_buff_fmtdesc_c4 -- Loads 4 formatted components using the resource descriptor from the buffer descriptor plus the encoded address and offsets into the vector destination
// [HW-observed] buff-carrier formatted load: the descriptor/address formation and the format-space readback are part of the buffer contract (format decode UNRESOLVED); the CPU model passes the loaded words through.
u64 r = ((u64)src[0] | ((u64)src[0+1] << 32));
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
