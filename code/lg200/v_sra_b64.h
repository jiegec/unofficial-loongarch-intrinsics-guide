// v_sra_b64 -- Arithmetically shifts a signed 64-bit lane value right: `d = a >> (b & 63)`.
//

u64 r = sra_b64(((u64)src[0] | ((u64)src[0+1] << 32)), ((u64)src[2] | ((u64)src[2+1] << 32)));
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
