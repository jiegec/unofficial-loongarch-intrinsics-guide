// load_v_buff_fmtdesc_c1 -- Reads one formatted element component through a buffer descriptor, with the element format declared by a resource record.
//
// buff-carrier formatted load: the descriptor/address formation and the format-space readback are part of the buffer contract (format decode UNRESOLVED); the CPU model passes the loaded words through.

dst[0] = src[0];
