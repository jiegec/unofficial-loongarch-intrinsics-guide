// v_fcvt_f32_u81 -- Extracts byte 1 of the 32-bit source, converts that unsigned byte to 32-bit floating point, and writes the result to the vector destination
// ("81" = unsigned byte lane 1; the byte index is decoded from the opcode, so
// v_fcvt_f32_u8{0,1,2,3} differ only in which source byte is converted.)
dst[0] = u32_f32((src[0] >> 8) & 0xffu, policy);
