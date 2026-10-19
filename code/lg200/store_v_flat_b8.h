// Stores an 8-bit bit pattern from the vector data source to the flat-memory address.
// [HW-observed] flat-carrier memory access: address formation and the bl1/bl2 behavior are part of the flat contract; the CPU model passes the data words through (wider tuples by their leading words).
dst[0] = src[0];
