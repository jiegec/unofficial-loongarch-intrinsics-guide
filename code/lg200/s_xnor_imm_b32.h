// s_xnor_imm_b32 -- Computes the complement of the exclusive-OR of a scalar source with a 32-bit literal: `dst = ~(src ^ imm)`.
//

dst[0] = ~(src[0] ^ src[1]);
