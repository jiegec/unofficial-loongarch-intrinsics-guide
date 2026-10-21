// v_ceil_f32 -- Rounds an f32 value toward positive infinity: `d = ceil_f32(a)`.
//
// [HW 2026-10-22] the flush is unconditional here too: ceil(0x1) returns 0x00000000 rather
// than 1.0, so a denormal input is zeroed before the rounding whatever the token says.

test::FpPolicy hw{};
hw.daz = true;
hw.ftz = true;
dst[0] = f32_ceil(src[0], hw);
