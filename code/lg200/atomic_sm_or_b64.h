// atomic_sm_or_b64 -- Atomically reads a 64-bit bit pattern from workgroup shared memory, computes new = old OR source, and writes new back to the same location
u64 updated = sm_or_b64(((u64)src[0] | ((u64)src[0+1] << 32)), ((u64)src[2] | ((u64)src[2+1] << 32)));
dst[0] = src[0];
dst[1] = src[1];
dst[2] = (u32)(updated & 0xffffffffu);
dst[3] = (u32)(updated >> 32);
