// atomic_v_tex_min_u32 -- Atomically reads an unsigned 32-bit integer from the addressed image resource, computes new is the lesser of old and source, and writes new back to the same location
// [HW-observed] tex-atomic: swap_b32 executes the RMW on the texture path, while the other 12 image-atomic rows behave as no-op execution holes ([HW] differential experiments); the CPU model computes the updated words.
u32 updated = v_tex_min_u32(src[0], src[1]);
dst[0] = src[0];
dst[1] = updated;
