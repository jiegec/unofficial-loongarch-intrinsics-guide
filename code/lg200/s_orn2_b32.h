// s_orn2_b32 -- ORs the first source with the complemented second: `d = a | ~b`.
//

dst[0] = src[0] | ~src[1];
