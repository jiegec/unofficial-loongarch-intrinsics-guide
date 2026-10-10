// s_cmp_imm -- Compares a 32-bit scalar with a literal for equality and writes `d = (a == b)`.
//

dst[0] = cmp_eq_u32(src[0], src[1]);
