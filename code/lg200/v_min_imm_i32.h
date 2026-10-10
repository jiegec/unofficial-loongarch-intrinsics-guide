// v_min_imm_i32 -- Selects the lesser of a signed i32 source and a 32-bit literal: `d = min(a, imm)`.
//

dst[0] = min_i32(src[0], src[1]);
