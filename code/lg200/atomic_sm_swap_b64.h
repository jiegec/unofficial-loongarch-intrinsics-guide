// atomic_sm_swap_b64 -- Replaces a u64 shared-memory element with the lane's source and returns the value that was displaced.
//

u64 updated = sm_swap_b64(((u64)src[0] | ((u64)src[0+1] << 32)), ((u64)src[2] | ((u64)src[2+1] << 32)));
dst[0] = src[0];
dst[1] = src[1];
dst[2] = (u32)(updated & 0xffffffffu);
dst[3] = (u32)(updated >> 32);
