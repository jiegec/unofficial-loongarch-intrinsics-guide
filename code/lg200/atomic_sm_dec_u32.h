// atomic_sm_dec_u32 -- Atomically reads an unsigned 32-bit integer from workgroup shared memory, computes new = old - 1 when 0 < old <= bound, otherwise the bound, and writes new back to the same location
u32 updated = sm_dec_u32(src[0], src[1]);
dst[0] = src[0];
dst[1] = updated;
