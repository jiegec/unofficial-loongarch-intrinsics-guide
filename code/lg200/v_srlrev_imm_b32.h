// v_srlrev_imm_b32 -- Shifts the literal logically right by a per-lane count from the vector source: `d = imm >> (a & 31)`.
//

dst[0] = src[1] >> (src[0] & 31u);
