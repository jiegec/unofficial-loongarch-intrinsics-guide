// v_mul_imm_f32 -- Multiplies an f32 vector value by the raw binary32 literal: `d = a * imm`.
//
test::FpPolicy hw{};
hw.daz = true;
hw.ftz = true;
dst[0] = f32_mul(src[0], src[1], hw);
