// atomic_sm_add_u64 -- Atomically adds the lane's source to the 64-bit shared-memory element: `new = old + src` modulo 2^64.
//

u64 updated = sm_add_u64(((u64)src[0] | ((u64)src[0+1] << 32)), ((u64)src[2] | ((u64)src[2+1] << 32)));
dst[0] = src[0];
dst[1] = src[1];
dst[2] = (u32)(updated & 0xffffffffu);
dst[3] = (u32)(updated >> 32);
