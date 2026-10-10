// v_xnor_b32 -- Complements the bitwise exclusive-OR of two 32-bit sources: `d = ~(a ^ b)`.
//

dst[0] = ~(src[0] ^ src[1]);
