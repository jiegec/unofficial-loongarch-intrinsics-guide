// s_rotrrev_imm_b32 -- Rotates a 32-bit literal right by a count taken from a scalar register: `d = rotr(imm, a & 31)`.
//

dst[0] = rotrrev_b32(src[0], src[1]);
