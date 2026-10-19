// load_s_flat_w1 -- Loads 1 32-bit word from the scalar flat-memory
// address and 20-bit offset into consecutive scalar registers.
// CPU model: the read is data-passthrough -- src[0] carries the
// words loaded by the flat path and dst mirrors them. Flat address
// formation is part of the memory contract and is not modeled at register
// level.
dst[0] = src[0];
