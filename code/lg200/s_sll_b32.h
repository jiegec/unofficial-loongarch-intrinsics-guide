// s_sll_b32 -- Shifts a 32-bit scalar value left by a count taken from a second source: `d = a << (b & 31)`.
//

dst[0] = src[0] << (src[1] & 31u);
