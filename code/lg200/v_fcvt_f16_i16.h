// v_fcvt_f16_i16 -- Recorded as converting the low 16 bits, read as signed, to binary16; on this silicon the destination register is not written.
//
// [HW 2026-11-03] Measured, not assumed: the destination is pre-loaded with
// 0xa5a5a5a5 before the instruction and that word is what reads back, on both
// pipes, for all 15 vectors - including the ones whose conversion result would
// be 0x3c00, 0xbc00 or 0.  The row now compares that invariant instead of
// recording a conversion that does not happen.  Whether the opcode writes some
// OTHER register is not established; what is established is that the register
// the row names and the carrier grades is untouched.

dst[0] = 0xa5a5a5a5u;
