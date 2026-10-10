// s_absdiff_u32 -- Computes the absolute difference of two unsigned 32-bit scalars: `d = max - min`.
//

dst[0] = src[0] >= src[1] ? src[0] - src[1] : src[1] - src[0];
