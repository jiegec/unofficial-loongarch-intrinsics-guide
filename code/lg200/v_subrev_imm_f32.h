// v_subrev_imm_f32 -- Subtracts an f32 register source from a literal: the literal is the minuend.
//
test::FpPolicy hw{};
hw.daz = true;
hw.ftz = true;
dst[0] = f32_revsub(src[0], src[1], hw);
