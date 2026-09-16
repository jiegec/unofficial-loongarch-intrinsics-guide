// atomic_sm_min_i32 -- Atomically reads a signed 32-bit integer from workgroup shared memory, computes new is the lesser of old and source, and writes new back to the same location
u32 updated = sm_min_i32(src[0], src[1]);
dst[0] = src[0];
dst[1] = updated;
