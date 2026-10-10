// s_select_b32 -- Selects between two values by the condition register: `d = cond != 0 ? A : b`.
//

dst[0] = src[2] != 0 ? src[0] : src[1];
