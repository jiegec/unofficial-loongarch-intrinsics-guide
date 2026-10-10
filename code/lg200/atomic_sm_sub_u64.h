// atomic_sm_sub_u64 -- Replaces a u64 shared-memory element with `old - src` modulo 2**64 and returns the old value.
//

u64 updated = sm_sub_u64(((u64)src[0] | ((u64)src[0+1] << 32)), ((u64)src[2] | ((u64)src[2+1] << 32)));
dst[0] = src[0];
dst[1] = src[1];
dst[2] = (u32)(updated & 0xffffffffu);
dst[3] = (u32)(updated >> 32);
