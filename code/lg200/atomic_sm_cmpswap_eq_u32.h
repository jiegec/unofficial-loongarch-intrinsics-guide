// atomic_sm_cmpswap_eq_u32 -- Atomically reads an unsigned 32-bit integer from workgroup shared memory, computes new = swap when old equals compare, otherwise old, and writes new back to the same location
u32 updated = sm_cmpswap_eq_u32(src[0], src[1], src[2]);
dst[0] = src[0];
dst[1] = updated;
