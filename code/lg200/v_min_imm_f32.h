// v_min_imm_f32 -- Selects the lesser of an f32 source and a 32-bit literal: `d = min(a, imm)`.
//

dst[0] = f32_min(src[0], src[1], policy);
