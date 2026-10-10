// s_sext_i32_i8 -- Sign-extends the low byte of a scalar source to i32: `d = (src[7] ? 0xffffff00 : 0) | src[7:0]`.
//

dst[0] = sext_i32_i8(src[0]);
