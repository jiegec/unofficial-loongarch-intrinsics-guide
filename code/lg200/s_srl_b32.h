// s_srl_b32 -- Logically shifts a 32-bit scalar value right with zero fill: `d = a >> (b & 31)`.
//

dst[0] = src[0] >> (src[1] & 31u);
