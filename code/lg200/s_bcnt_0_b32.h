// s_bcnt_0_b32 -- Counts the zero bits of a 32-bit scalar: `d = popcount(~a)`.
//

dst[0] = bcnt0_u32(src[0]);
