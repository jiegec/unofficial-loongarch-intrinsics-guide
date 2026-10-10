// v_pack_imm_u16_u32 -- Packs the low 16 bits of a u32 lane value and of the literal: `d = pack(u16(a), u16(imm))`.
//

dst[0] = pack_u16_u32(src[0], src[1]);
