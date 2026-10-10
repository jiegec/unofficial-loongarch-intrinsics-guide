// v_sub_f32 -- Subtracts the second f32 source from the first: `d = a - b`.
//

test::FpPolicy hw{};
hw.daz = true;
hw.ftz = true;
dst[0] = f32_sub(src[0], src[1], hw);
