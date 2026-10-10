// v_pack_imm_i16_i32 -- Packs the low 16 bits of an i32 lane value and of the literal: `d = pack(i16(a), i16(imm))`.
//

dst[0] = pack_i16_i32(src[0], src[1]);
