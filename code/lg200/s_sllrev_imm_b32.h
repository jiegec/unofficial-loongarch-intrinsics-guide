// s_sllrev_imm_b32 -- Shifts a 32-bit literal left by a register-sourced count: `d = imm << (a & 31)`.
//

dst[0] = src[1] << (src[0] & 31u);
