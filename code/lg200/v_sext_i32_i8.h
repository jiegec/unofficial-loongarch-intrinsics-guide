// v_sext_i32_i8 -- Sign-extends the low byte of each lane to a signed 32-bit value: `d = (i32)(i8)(a & 0xff)`.
//

dst[0] = sext_i32_i8(src[0]);
