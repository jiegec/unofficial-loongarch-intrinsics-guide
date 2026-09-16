// atomic_sm_rsub_u32 -- Atomically reads an unsigned 32-bit integer from workgroup shared memory, computes new = source - old modulo the operand width, and writes new back to the same location
u32 updated = sm_rsub_u32(src[0], src[1]);
dst[0] = src[0];
dst[1] = updated;
