// v_fcvt_f16_u16 -- Recorded as converting the low 16 bits, read as unsigned, to binary16; on this silicon the destination register is not written.
//
// [HW 2026-11-03] Same measurement as v_fcvt_f16_i16: a pre-loaded
// 0xa5a5a5a5 survives all 16 vectors on both pipes, so the row compares the
// no-write invariant.  The conversion the title describes is not observable here.

dst[0] = 0xa5a5a5a5u;
