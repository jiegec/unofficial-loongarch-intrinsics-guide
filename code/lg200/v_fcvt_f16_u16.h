// v_fcvt_f16_u16 -- Converts the unsigned 16-bit integer source to a 16-bit floating-point value and writes the result to the vector destination
// LG200 NO-OP HOLE: this LG210-legacy encoding decodes but executes empty on
// LG200; the destination register is left unchanged (undefined value, run-to-run
// varying garbage). The u16->f16 conversion below is LG210 intent only.
dst[0] = fcvt_f16_u16(src[0]);
