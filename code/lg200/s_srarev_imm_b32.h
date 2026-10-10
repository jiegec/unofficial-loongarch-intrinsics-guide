// s_srarev_imm_b32 -- Arithmetically shifts a 32-bit literal right by a register-sourced count: `d = (int32)imm >> (a & 31)`.
//

dst[0] = srarev_b32(src[0], src[1]);
