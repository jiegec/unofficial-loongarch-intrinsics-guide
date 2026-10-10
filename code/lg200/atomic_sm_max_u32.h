// atomic_sm_max_u32 -- Atomically keeps the greater of the 32-bit unsigned contents and the source: `new = max_u32(old, src)`.
//

u32 updated = sm_max_u32(src[0], src[1]);
dst[0] = src[0];
dst[1] = updated;
