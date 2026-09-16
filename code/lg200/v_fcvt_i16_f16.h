// v_fcvt_i16_f16 -- Converts the 16-bit floating-point value source to a signed 16-bit integer and writes the result to the vector destination
// LG200 NO-OP HOLE: this LG210-legacy encoding decodes but executes empty on
// LG200; the destination register is left unchanged (undefined value, run-to-run
// varying garbage). The f16->i16 conversion below is LG210 intent only.
dst[0] = fcvt_i16_f16(src[0]);
