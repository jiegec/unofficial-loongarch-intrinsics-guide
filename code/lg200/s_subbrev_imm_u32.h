// s_subbrev_imm_u32 -- Subtracts a scalar source and an incoming borrow from a 32-bit literal: `dst = imm - src - borrow_in`.
//

dst[0] = src[1] - src[0] - (src[2] != 0u ? 1u : 0u);
