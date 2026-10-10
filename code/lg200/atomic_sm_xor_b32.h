// atomic_sm_xor_b32 -- Replaces a u32 shared-memory element with the bitwise XOR of its contents and the source, returning the old value.
//

u32 updated = sm_xor_b32(src[0], src[1]);
dst[0] = src[0];
dst[1] = updated;
