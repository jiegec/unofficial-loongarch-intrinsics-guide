// v_absdiff_imm_u32 -- Computes the unsigned distance between a u32 lane and a 32-bit literal.
//

dst[0] = src[0] >= src[1] ? src[0] - src[1] : src[1] - src[0];
