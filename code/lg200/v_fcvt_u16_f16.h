// v_fcvt_u16_f16 -- Converts the 16-bit floating-point value source to an unsigned 16-bit integer and writes the result to the vector destination
// LG200 NO-OP HOLE: this LG210-legacy encoding decodes but executes empty on
// LG200; the destination register is left unchanged (undefined value, run-to-run
// varying garbage). The f16->u16 conversion below is LG210 intent only.
dst[0] = fcvt_u16_f16(src[0]);
