// s_bitcmprev1_imm_b32 -- Tests whether the immediate-selected source bit
// is one and writes the result to the scalar destination.
// Reversed bit compare: bit (src[0] & 31) of the immediate (src[1]) is
// tested; the result is the SCC materialization: 0xffff for true, 0 for
// false.
dst[0] = bitcmprev1(src[1], src[0]);
