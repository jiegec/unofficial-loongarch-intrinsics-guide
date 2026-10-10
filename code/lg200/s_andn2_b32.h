// s_andn2_b32 -- Computes `d = a & ~b`: the second source is complemented before the AND.
//

dst[0] = src[0] & ~src[1];
