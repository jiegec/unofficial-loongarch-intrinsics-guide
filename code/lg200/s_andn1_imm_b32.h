// s_andn1_imm_b32 -- Computes the bitwise AND of a complemented scalar and a literal: `d = ~a & b`.
//

dst[0] = ~src[0] & src[1];
