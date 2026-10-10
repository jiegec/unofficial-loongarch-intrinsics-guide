// v_max_imm_u32 -- Selects the greater of an unsigned 32-bit value and a literal: `d = max(a, imm)`.
//

dst[0] = max_u32(src[0], src[1]);
