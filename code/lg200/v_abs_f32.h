// v_abs_f32 -- Takes the magnitude of an f32 value: a denormal input flushes to zero, anything
// else loses its sign bit: `d = flush(a) & 0x7fffffff`.
//
// [HW 2026-11-03] The first line used to promise a pure bit clear ("d = a & 0x7fffffff") and
// the body below contradicts it for exactly those inputs: 0x807fffff is a denormal, so a bit
// clear would give 0x007fffff while the flush gives 0.  The title says what the row does now.
// Which of the two the hardware does on a denormal is NOT settled by this row's vectors - they
// were all chosen to agree with the flush.
//
// [HW 2026-10-22] a denormal input comes back zero even under a bare token, so the flush is
// unconditional here: 0x1 returns 0x00000000 and 0x80000001 also returns 0x00000000, where
// passing the bits through would have returned 0x007fffff and 0x00000001.

u32 v = src[0];
v = (v & 0x7f800000u) == 0u ? (v & 0x80000000u) : v;  // a denormal flushes to signed zero
dst[0] = v & 0x7fffffffu;
