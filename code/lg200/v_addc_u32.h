// v_addc_u32 -- Adds two u32 values plus a carry-in: `d = a + b + cin`, with carry-out.
//
// The third source is the VCC pair (s6:s7), and the instruction consumes it
// twice.  Its low sixteen bits are sixteen independent carry-ins - bit i is
// lane i's carry-in [HW 2026-11-03: 0x0001 reaches lane 0, 0x0000fffe reaches
// lanes 1..15, 0x00010000 and 0x80000000 reach no lane] - and the instruction
// writes the sixteen carry-outs back into the same pair's low word, bit i =
// lane i's carry-out, leaving s7 untouched.
//
// dst[1] is that scalar word, so it is exact.  dst[0] is the carrier's
// readback, not one lane: a case stages one cell and all sixteen lanes store
// to it, so the host reads the OR over the lanes.  With s = a + b a lane reads
// s (carry-in 0) or s + 1 (carry-in 1), so the aggregate is s when no lane
// carries in, s + 1 when all sixteen do, and s | (s + 1) in between.

u32 cin = src[2] & 0xffffu;
u32 s = src[0] + src[1];
dst[0] = (cin == 0u)      ? s
       : (cin == 0xffffu) ? s + 1u
       : (s | (s + 1u));

u32 carry = 0u;
for (u32 lane = 0u; lane < 16u; ++lane) {
  u64 wide = (u64)src[0] + src[1] + ((cin >> lane) & 1u);
  carry |= (u32)(wide >> 32) << lane;
}
dst[1] = carry;
