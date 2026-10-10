// s_rotr_b32 -- Rotates a 32-bit value right by a count taken from the second source: `d = rotr32(a, b & 31)`.
//

dst[0] = rotr_b32(src[0], src[1]);
