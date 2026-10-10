// atomic_sm_min_i64 -- Atomically keeps the lesser of the 64-bit signed contents and the source: `new = min_s64(old, src)`.
//

u64 updated = sm_min_i64(((u64)src[0] | ((u64)src[0+1] << 32)), ((u64)src[2] | ((u64)src[2+1] << 32)));
dst[0] = src[0];
dst[1] = src[1];
dst[2] = (u32)(updated & 0xffffffffu);
dst[3] = (u32)(updated >> 32);
