// s_wqm_b32 -- Expands every nonzero group of four adjacent bits into an all-ones group: `d = OR of 0xf << 4k`.
//

dst[0] = wqm(src[0]);
