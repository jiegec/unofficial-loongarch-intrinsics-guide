// s_max_imm_u32 -- Selects the greater of an unsigned 32-bit scalar source and a 32-bit literal: `d = max(a, b)`.
//

dst[0] = max_u32(src[0], src[1]);
