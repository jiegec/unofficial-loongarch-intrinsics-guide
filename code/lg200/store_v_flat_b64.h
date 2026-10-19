// Stores a 64-bit bit pattern from the vector data source to the flat-memory address.
// [HW-observed] flat-carrier memory access: address formation and the bl1/bl2 behavior are part of the flat contract; the CPU model passes the data words through (wider tuples by their leading words).
u64 r = ((u64)src[0] | ((u64)src[0+1] << 32));
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
