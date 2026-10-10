// s_cmp_imm_lt_u32 -- Compares a scalar value with a 32-bit literal as unsigned integers and writes the less-than predicate: `d = (a < b)`.
//

dst[0] = cmp_lt_u32(src[0], src[1]);
