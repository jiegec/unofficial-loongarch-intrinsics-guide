// v_threadid_u32 -- Writes each lane's unsigned thread identifier, its position within the wave.
//
// [HW 2026-11-03] The body states lane 0's answer, which is what a one-lane
// launch reads back.  Wider launches read the OR of the ids: 2 lanes give
// 0x00000001, 4 give 0x00000003, 8 give 0x00000007 and 16 give 0x0000000f on both
// pipes - (1<<lanes)-1.  The GPU carrier derives that word from the launch width;
// the offline oracle below stays the one-lane instance.

dst[0] = 0u;
