// load_s_flat_w4 -- Loads four consecutive 32-bit words from a flat address pair plus a 20-bit offset.
//

u64 r = ((u64)src[0] | ((u64)src[0+1] << 32));
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
