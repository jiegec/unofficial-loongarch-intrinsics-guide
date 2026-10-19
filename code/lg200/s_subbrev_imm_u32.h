// s_subbrev_imm_u32 -- Subtracts the first source and the low borrow-in
// bit from the second source and writes the result to the scalar
// destination.
// [HW-verified 2026-08-23] The borrow-in is the SCC state, not src[2]&1:
// the carrier stages the third source through a scalar move whose SCC
// equals (value != 0), and the hardware reads SCC as borrow-in.  The
// observable contract is dst = src[1] - src[0] - (src[2] != 0 ? 1 : 0).
dst[0] = src[1] - src[0] - (src[2] != 0u ? 1u : 0u);
