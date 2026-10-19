// load_s_flat_w16 -- Loads 16 32-bit words from the scalar flat-memory
// address and 20-bit offset into consecutive scalar registers.
// CPU model: the read is data-passthrough -- src[0..1] carries the
// words loaded by the flat path and dst mirrors them. Flat address
// formation is part of the memory contract and is not modeled at register
// level.
u64 r = ((u64)src[0] | ((u64)src[0+1] << 32));
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
