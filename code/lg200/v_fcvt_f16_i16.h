// v_fcvt_f16_i16 -- Converts the signed 16-bit integer source to a 16-bit floating-point value and writes the result to the vector destination
// LG200 NO-OP HOLE: this LG210-legacy encoding decodes but executes empty on
// LG200; the destination register is left unchanged (hardware leaves an
// undefined value, observed as run-to-run varying garbage). The i16->f16
// conversion described below is LG210 intent only and is not observable on
// LG200 hardware.
dst[0] = fcvt_f16_i16(src[0]);
