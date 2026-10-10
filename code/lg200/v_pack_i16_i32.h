// v_pack_i16_i32 -- Packs the low 16 bits of two i32 values into one word: `d = { a[15:0], b[15:0] }`.
//

dst[0] = pack_i16_i32(src[0], src[1]);
