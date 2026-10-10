// s_bfm_b32 -- Builds a contiguous bit mask from a width and an offset: `d = ((1 << width) - 1) << offset`.
//

dst[0] = bfm_b32(src[0], src[1]);
