// v_sll_b64 -- Shifts a 64-bit lane value left by a 64-bit count: `d = a << (b & 63)`.
//

u64 r = ((u64)src[2] | ((u64)src[2+1] << 32)) < 64 ? ((u64)src[0] | ((u64)src[0+1] << 32)) << (((u64)src[2] | ((u64)src[2+1] << 32)) & 63u) : 0;
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
