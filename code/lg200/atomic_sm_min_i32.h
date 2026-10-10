// atomic_sm_min_i32 -- Atomically keeps the lesser of the 32-bit signed contents and the source: `new = min_s32(old, src)`.
//

u32 updated = sm_min_i32(src[0], src[1]);
dst[0] = src[0];
dst[1] = updated;
