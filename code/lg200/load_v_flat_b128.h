// Loads a 128-bit bit pattern from the flat-memory address into the vector destination.
// [HW-observed] flat-carrier memory access: address formation and the bl1/bl2 behavior are part of the flat contract; the CPU model passes the data words through (wider tuples by their leading words).
u64 lo = (u64)src[0] | ((u64)src[1] << 32);
u64 hi = (u64)src[2] | ((u64)src[3] << 32);
dst[0] = (u32)(lo & 0xffffffffu);
dst[1] = (u32)(lo >> 32);
dst[2] = (u32)(hi & 0xffffffffu);
dst[3] = (u32)(hi >> 32);
