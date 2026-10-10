// v_max_imm_f32 -- Selects the greater of an f32 source and a 32-bit literal: `d = max(a, imm)`.
//

dst[0] = f32_max(src[0], src[1], policy);
