// atomic_v_tex_cmpswap_b32 -- Atomically reads a 32-bit bit pattern from the addressed image resource, computes new = swap when old equals compare, otherwise old, and writes new back to the same location
// [HW-observed] tex-atomic: swap_b32 executes the RMW on the texture path, while the other 12 image-atomic rows behave as no-op execution holes ([HW] differential experiments); the CPU model computes the updated words.
u32 updated = v_tex_cmpswap_b32(src[0], src[1], src[2]);
dst[0] = src[0];
dst[1] = updated;
