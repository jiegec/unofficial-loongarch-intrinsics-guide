// atomic_sm_inc_u32 -- Atomically steps the bounded 32-bit counter up: `new = (old < b) ? Old + 1 : 0`.
//

u32 updated = sm_inc_u32(src[0], src[1]);
dst[0] = src[0];
dst[1] = updated;
