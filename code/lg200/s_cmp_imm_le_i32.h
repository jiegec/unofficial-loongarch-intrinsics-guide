// s_cmp_imm_le_i32 -- Compares a signed 32-bit scalar with a literal and writes `d = (a <= b)`.
//

dst[0] = ((s32)src[0] <= (s32)src[1]) ? 0xffffu : 0u;
