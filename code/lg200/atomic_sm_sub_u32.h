// atomic_sm_sub_u32 -- Replaces a u32 shared-memory element with `old - src` modulo 2**32 and returns the old value.
//

u32 updated = sm_sub_u32(src[0], src[1]);
dst[0] = src[0];
dst[1] = updated;
