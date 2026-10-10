// v_absdiff_u32 -- Computes the distance between two u32 values: `d = |a - b|`.
//

dst[0] = src[0] >= src[1] ? src[0] - src[1] : src[1] - src[0];
