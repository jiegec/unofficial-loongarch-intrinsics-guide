// v_max_imm_i32 -- Selects the greater of a signed i32 source and a 32-bit literal: `d = max(a, imm)`.
//

dst[0] = max_i32(src[0], src[1]);
