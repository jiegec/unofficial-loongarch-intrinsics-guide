// v_subbrev_u32 -- Subtracts the first source from the second source under the VCC borrow-in state and writes the result to the vector destination
// [HW-verified 2026-08-25] EXACT mirror of v_subb_u32 with the operand
// roles swapped (r = src1 - src0) and the same low30 borrow trichotomy:
//   low30 == 0            -> plain r
//   low30 == 0x3fffffff   -> r - 1
//   any other low30       -> r | (r - 1)  (round up to next Mersenne)
// Probed: (1,5,1)->7, (1,5,0x3fffffff)->3, (1,5,0)->4, (1,0x10001,1)->0x1ffff.
// Supersedes the older "hardware ignores the borrow-in bit" note (based on
// A sparse {0,1} borrow set the old rows never exercised).
dst[0] = (src[2] & 0x3fffffffu) == 0x3fffffffu
             ? src[1] - src[0] - 1u
             : (src[2] & 0x3fffffffu) == 0u
                   ? src[1] - src[0]
                   : (src[1] - src[0]) | (src[1] - src[0] - 1u);
