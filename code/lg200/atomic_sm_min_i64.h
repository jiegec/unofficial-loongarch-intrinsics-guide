// atomic_sm_min_i64 -- Atomically reads a signed 64-bit integer from workgroup shared memory, computes new is the lesser of old and source, and writes new back to the same location
u64 updated = sm_min_i64(((u64)src[0] | ((u64)src[0+1] << 32)), ((u64)src[2] | ((u64)src[2+1] << 32)));
dst[0] = src[0];
dst[1] = src[1];
dst[2] = (u32)(updated & 0xffffffffu);
dst[3] = (u32)(updated >> 32);
