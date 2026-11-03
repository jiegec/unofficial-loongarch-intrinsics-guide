// v_subb_u32 -- Subtracts the second u32 source from the first under a borrow state held in the third source.
//
// The third source is the VCC pair (s6:s7), and the instruction consumes it
// twice.  Its low sixteen bits are sixteen independent borrow-ins - bit i is
// lane i's borrow-in - and the instruction writes the sixteen borrow-outs back
// into the same pair's low word, bit i = lane i's borrow-out, leaving s7
// untouched.
//
// dst[1] is that scalar word, so it is exact.  dst[0] is the carrier's
// readback, not one lane: a case stages one cell and all sixteen lanes store
// to it, so the host reads the OR over the lanes.  With r = a - b a lane reads
// r (borrow-in 0) or r - 1 (borrow-in 1), so the aggregate is r when no lane
// borrows in, r - 1 when all sixteen do, and r | (r - 1) in between.

u32 bin = src[2] & 0xffffu;
u32 r = src[0] - src[1];
dst[0] = (bin == 0u)      ? r
       : (bin == 0xffffu) ? r - 1u
       : (r | (r - 1u));

u32 borrow = 0u;
for (u32 lane = 0u; lane < 16u; ++lane) {
  u64 sub = (u64)src[1] + ((bin >> lane) & 1u);
  borrow |= (u32)((u64)src[0] < sub) << lane;
}
dst[1] = borrow;
