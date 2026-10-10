// s_andn1_b32 -- Computes `d = ~a & b`: the first source is complemented before the AND.
//

dst[0] = ~src[0] & src[1];
