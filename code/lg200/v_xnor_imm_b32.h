// v_xnor_imm_b32 -- Complements the bitwise XOR of a b32 source and a literal: `d = ~(a ^ b)`.
//

dst[0] = ~(src[0] ^ src[1]);
