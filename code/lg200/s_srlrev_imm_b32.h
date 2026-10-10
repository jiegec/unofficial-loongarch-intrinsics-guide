// s_srlrev_imm_b32 -- Logically shifts a 32-bit literal right by a count taken from a scalar register: `dst = imm >> (src & 31)`.
//

dst[0] = src[1] >> (src[0] & 31u);
