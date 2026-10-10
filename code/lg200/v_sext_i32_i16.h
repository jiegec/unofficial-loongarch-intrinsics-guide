// v_sext_i32_i16 -- Sign-extends the low half-word of each lane to a signed 32-bit value: `d = (i32)(i16)(a & 0xffff)`.
//

dst[0] = sext_i32_i16(src[0]);
