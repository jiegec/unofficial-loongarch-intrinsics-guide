// v_rotrrev_imm_b32 -- Rotates the literal right by a per-lane count taken from the vector source: `d = rotr(imm, a & 31)`.
//

dst[0] = rotrrev_b32(src[0], src[1]);
