// v_mbcnt_u32 -- Builds a mask from the population count of one source and ORs it into a second source.
//
// [HW 2026-11-03] The popcount field is not a fixed slice of the source: it is
// the launch's lane count minus one bits.  Same operands, one instruction, four
// widths - (a=0x7fffffff, b=0) returns 0x3 at four lanes, 0x7 at eight, 0xf at
// sixteen and 0x1f at thirty-two, which are the popcounts of 3, 7, 15 and 31 set
// bits.  The body below evaluates the four-lane instance (the three-bit window
// the 2026-10-13 probe measured on the compute pipe); the GPU carrier
// re-evaluates it at the launch's own width, because the answer is not a
// function of the operands alone.
//
// The widths are not one rule everywhere: in compute, four lanes and below agree
// with the table, eight lanes agree on 40 of 41 vectors (the exception is the
// extension step, a=0xff b=8 reading 0x0f where the window model extends to
// 0x1f, though the same pair reads 0x0b at four lanes), and sixteen and
// thirty-two agree on 25 of 41 - so those launches observe instead of grading.
// Graphics runs one sixteen-lane wave and agrees on all 41.

dst[0] = mbcnt_u32(src[0], src[1]);
