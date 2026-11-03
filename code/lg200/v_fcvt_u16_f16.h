// v_fcvt_u16_f16 -- Recorded as converting a half-precision source to an unsigned 16-bit integer; on this silicon the destination register is not written.
//
// [HW 2026-11-03] A pre-loaded 0xa5a5a5a5 survives all 17 vectors on both pipes,
// so the row compares the no-write invariant.

dst[0] = 0xa5a5a5a5u;
