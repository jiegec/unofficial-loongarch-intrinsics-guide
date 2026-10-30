// v_select_b32 -- Selects one of two 32-bit lane values under a per-lane condition: `d = c ? A : b`.
//
// A is src[0], b is src[1], and c is src[2] - the GA11 operand, a general
// source selector, so it names either an ordinary scalar register (the
// canonical form stages it in s0) or a lane-mask register.  The condition is
// read per lane: bit <lane> set takes A, clear takes b.
//
dst[0] = select_b32(src[0], src[1], src[2]);
