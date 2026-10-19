// atomic_sm_and_b32 -- Atomically reads a 32-bit bit pattern from workgroup shared memory, computes new = old AND source, and writes new back to the same location
u32 updated = sm_and_b32(src[0], src[1]);
dst[0] = src[0];
dst[1] = updated;
