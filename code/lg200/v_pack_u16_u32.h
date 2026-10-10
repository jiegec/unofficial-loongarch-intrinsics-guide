// v_pack_u16_u32 -- Packs the low 16 bits of two u32 values into one word: `d = { a[15:0], b[15:0] }`.
//

dst[0] = pack_u16_u32(src[0], src[1]);
