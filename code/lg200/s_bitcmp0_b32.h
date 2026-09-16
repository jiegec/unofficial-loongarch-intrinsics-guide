// s_bitcmp0_b32 -- Tests whether the selected source bit is zero and
// writes 0xffff for true or zero for false to the scalar destination.
// The result is the SCC materialization of (bit == 0) at the encoded
// selector (src[1] & 31).
dst[0] = bitcmp0_b32(src[0], src[1]);
