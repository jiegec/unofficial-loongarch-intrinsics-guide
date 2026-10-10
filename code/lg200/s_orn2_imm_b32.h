// s_orn2_imm_b32 -- ORs the scalar source with the complemented 32-bit literal: `d = a | ~b`.
//

dst[0] = src[0] | ~src[1];
