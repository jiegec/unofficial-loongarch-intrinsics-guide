// atomic_sm_cmpswap_gt_f32 -- Atomically reads a 32-bit floating-point value from workgroup shared memory, computes new = source when old is greater than source, otherwise old, and writes new back to the same location
u32 updated = sm_cmpswap_gt_f32(src[0], src[1]);
dst[0] = src[0];
dst[1] = updated;
