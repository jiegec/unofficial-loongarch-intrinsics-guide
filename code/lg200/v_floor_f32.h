// v_floor_f32 -- Rounds each lane's f32 source toward negative infinity: `d = floor(a)`.
//
// [HW 2026-10-22] the flush is unconditional, as it is in v_abs_f32 and v_fract_f32: a
// denormal input comes back as signed zero even under a token without daz, so
// floor(0x80000001) returns 0x80000000 (-0) rather than -1.0.

test::FpPolicy hw{};
hw.daz = true;
hw.ftz = true;
dst[0] = f32_floor(src[0], hw);
