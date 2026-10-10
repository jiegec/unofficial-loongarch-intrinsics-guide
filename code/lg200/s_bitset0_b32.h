// s_bitset0_b32 -- Clears a selected bit of a 32-bit scalar and writes the modified word.
//

dst[0] = src[0] & ~(1u << (src[1] & 31u));
