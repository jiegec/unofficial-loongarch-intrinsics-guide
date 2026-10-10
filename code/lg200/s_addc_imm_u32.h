// s_addc_imm_u32 -- Adds an unsigned 32-bit scalar, a literal and the incoming carry: `d = a + b + carry_in`.
//

dst[0] = s_addc_u32(src[0], src[1], src[2]);
