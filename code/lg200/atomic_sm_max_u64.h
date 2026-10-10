// atomic_sm_max_u64 -- Atomically keeps the greater of the 64-bit unsigned contents and the source: `new = max_u64(old, src)`.
//

u64 updated = sm_max_u64(((u64)src[0] | ((u64)src[0+1] << 32)), ((u64)src[2] | ((u64)src[2+1] << 32)));
dst[0] = src[0];
dst[1] = src[1];
dst[2] = (u32)(updated & 0xffffffffu);
dst[3] = (u32)(updated >> 32);
