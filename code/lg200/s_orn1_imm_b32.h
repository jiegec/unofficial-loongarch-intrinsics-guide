// s_orn1_imm_b32 -- ORs the complemented scalar source with a 32-bit literal: `d = ~a | b`.
//

dst[0] = ~src[0] | src[1];
