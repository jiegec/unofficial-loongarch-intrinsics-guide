// atomic_sm_min_u32 -- Replaces a u32 shared-memory element with `min(old, src)`, compared unsigned, and returns the old value.
//

u32 updated = sm_min_u32(src[0], src[1]);
dst[0] = src[0];
dst[1] = updated;
