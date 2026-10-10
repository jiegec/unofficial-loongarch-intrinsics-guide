// s_subb_u32 -- Subtracts a second scalar value and an incoming borrow from a first: `d = a - b - borrow_in`.
//

dst[0] = src[0] - src[1] - (src[2] != 0u ? 1u : 0u);
