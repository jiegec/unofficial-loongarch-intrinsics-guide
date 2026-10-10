// v_mul_f32 -- Multiplies two f32 values, rounding to nearest: `d = a * b`.
//

test::FpPolicy hw{};
hw.daz = true;
hw.ftz = true;
dst[0] = f32_mul(src[0], src[1], hw);
