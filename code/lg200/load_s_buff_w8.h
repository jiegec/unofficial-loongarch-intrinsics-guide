// load_s_buff_w8 -- Loads 8 32-bit words from the scalar buffer
// resource selected by the descriptor, scalar address, and 20-bit offset
// into consecutive scalar registers.
// CPU model: the read is data-passthrough -- src[0..1] carries the loaded
// words (the carrier models the wider tuples by their leading pair) and
// dst mirrors them. The descriptor/address formation is part of the memory
// contract ([HW-verified] on the smem carrier) and is not modeled at
// register level.
u64 r = ((u64)src[0] | ((u64)src[0+1] << 32));
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
