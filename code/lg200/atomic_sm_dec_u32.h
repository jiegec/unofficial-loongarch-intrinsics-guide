// atomic_sm_dec_u32 -- Atomically steps the bounded 32-bit counter down: `new = (old > 0 && old <= b) ? Old - 1 : b`.
//

u32 updated = sm_dec_u32(src[0], src[1]);
dst[0] = src[0];
dst[1] = updated;
