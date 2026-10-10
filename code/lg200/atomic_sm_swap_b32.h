// atomic_sm_swap_b32 -- Replaces a u32 shared-memory element with the lane's source and returns the value that was displaced.
//

u32 updated = sm_swap_b32(src[0], src[1]);
dst[0] = src[0];
dst[1] = updated;
