// s_absdiff_imm_u32 -- Computes the absolute difference of an unsigned 32-bit scalar and a literal: `d = |a - b|`.
//

dst[0] = src[0] >= src[1] ? src[0] - src[1] : src[1] - src[0];
