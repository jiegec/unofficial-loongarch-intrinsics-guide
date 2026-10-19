// Loads a signed 8-bit integer from the flat-memory address into the vector destination and sign-extends it to 32 bits.
// [HW-observed] flat-carrier memory access: address formation and the bl1/bl2 behavior are part of the flat contract; the CPU model passes the data words through (wider tuples by their leading words).
dst[0] = src[0];
