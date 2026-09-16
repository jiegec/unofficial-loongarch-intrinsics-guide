// s_bitcmp1_b32 -- Tests whether the selected source bit is one and
// writes 0xffff for true or zero for false to the scalar destination.
// The result is the SCC materialization of (bit == 1) at the encoded
// selector (src[1] & 31).
dst[0] = bitcmp1_b32(src[0], src[1]);
