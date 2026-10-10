// atomic_v_tex_swap_b32 -- Atomically exchanges a 32-bit source with the value in an image element: `new = src`.
//
// tex-atomic: swap_b32 executes the RMW on the texture path, while the other 12 image-atomic rows behave as no-op execution holes ([HW] differential experiments); the CPU model computes the updated words.

u32 updated = v_tex_swap_b32(src[0], src[1]);
dst[0] = src[0];
dst[1] = updated;
