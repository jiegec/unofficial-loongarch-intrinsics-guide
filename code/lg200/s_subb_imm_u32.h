// s_subb_imm_u32 -- Subtracts a 32-bit literal and an incoming borrow from a scalar source: `dst = src - imm - borrow_in`.
//

dst[0] = src[0] - src[1] - (src[2] != 0u ? 1u : 0u);
