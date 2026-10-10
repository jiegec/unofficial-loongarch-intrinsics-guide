// v_srarev_imm_b32 -- Shifts the literal arithmetically right by a per-lane count from the vector source: `d = sra(imm, a & 31)`.
//

dst[0] = srarev_b32(src[0], src[1]);
