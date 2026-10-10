// atomic_sm_inc_u64 -- Atomically steps the bounded 64-bit counter up: `new = (old < b) ? Old + 1 : 0`.
//

u64 updated = sm_inc_u64(((u64)src[0] | ((u64)src[0+1] << 32)), ((u64)src[2] | ((u64)src[2+1] << 32)));
dst[0] = src[0];
dst[1] = src[1];
dst[2] = (u32)(updated & 0xffffffffu);
dst[3] = (u32)(updated >> 32);
