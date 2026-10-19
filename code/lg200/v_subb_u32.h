// v_subb_u32 -- Subtracts the second source from the first source under the VCC borrow-in state.
// [HW-verified 2026-08-25 truth table, ~45 probe points] The VCC borrow
// operand gates on its LOW 30 BITS with a three-branch outcome:
//   low30 == 0            -> plain subtraction  r = src0 - src1
//   low30 == 0x3fffffff   -> r - 1  (full borrow, all lanes borrow)
//   any other low30       -> r | (r - 1)  (partial borrow: the difference
//                            rounds UP to the next Mersenne number 2^k - 1;
//                            r == 0 wraps to 0xffffffff)
// Observed: (5,1,1)->7, (9,1,1)->15, (0x10001,1,1)->0x1ffff (partial);
// (5,1,0x3fffffff)->3, (0x10001,1,0x3fffffff)->0xffff (full);
// (5,1,0)->4, (0x10001,1,0)->0x10000 (none).  The old sparse {0,1}-borrow
// model mispredicted every low30 in between; the partial-branch Mersenne
// rounding (r | (r-1)) is a hardware arithmetic quirk (UNRESOLVED why).
dst[0] = (src[2] & 0x3fffffffu) == 0x3fffffffu
             ? src[0] - src[1] - 1u
             : (src[2] & 0x3fffffffu) == 0u
                   ? src[0] - src[1]
                   : (src[0] - src[1]) | (src[0] - src[1] - 1u);
