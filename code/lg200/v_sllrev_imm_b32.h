// v_sllrev_imm_b32 -- Shifts the literal left by a per-lane count taken from the vector source: `d = imm << (a & 31)`.
//

dst[0] = src[1] << (src[0] & 31u);
