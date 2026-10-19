// load_s_buff_w1 -- Loads 1 32-bit word from the scalar buffer
// resource selected by the descriptor, scalar address, and 20-bit offset
// into consecutive scalar registers.
// CPU model: the read is data-passthrough -- src[0] carries the word
// loaded by the memory path and dst mirrors it. The descriptor/address
// formation is part of the memory contract ([HW-verified] on the smem
// carrier) and is not modeled at register level.
dst[0] = src[0];
