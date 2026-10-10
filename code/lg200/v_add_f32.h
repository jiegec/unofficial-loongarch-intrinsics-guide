// v_add_f32 -- Adds two f32 values: `d = a + b`.
//

test::FpPolicy hw{};
hw.daz = true;
hw.ftz = true;
dst[0] = f32_add(src[0], src[1], hw);
