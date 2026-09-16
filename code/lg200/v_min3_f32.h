// v_min3_f32 -- Selects the least of the three sources and writes the result to the vector destination
// [HW 2026-08-24] f32 denormal inputs flush to signed zero before
// min3 (hardware compares flushed operands).
u32 a0 = (src[0] & 0x7f800000u) == 0u ? (src[0] & 0x80000000u) : src[0];
u32 a1 = (src[1] & 0x7f800000u) == 0u ? (src[1] & 0x80000000u) : src[1];
u32 a2 = (src[2] & 0x7f800000u) == 0u ? (src[2] & 0x80000000u) : src[2];
dst[0] = f32_min3(a0, a1, a2, policy);
