// atomic_v_tex_xor_b32 -- Atomically XORs a 32-bit source pattern into an image element: `new = old ^ src`.
//
// tex-atomic: swap_b32 executes the RMW on the texture path, while the other 12 image-atomic rows behave as no-op execution holes ([HW] differential experiments); the CPU model computes the updated words.

u32 updated = v_tex_xor_b32(src[0], src[1]);
dst[0] = src[0];
dst[1] = updated;
