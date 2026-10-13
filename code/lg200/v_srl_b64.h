// v_srl_b64 -- Logically shifts a 64-bit lane value right, filling with zeros: `d = a >> (b & 63)`.
//

u64 r = ((u64)src[0] | ((u64)src[0+1] << 32)) >> (((u64)src[2] | ((u64)src[2+1] << 32)) & 63u);
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
