// load_v_flat_b96 -- Loads a per-lane 96-bit element from a flat address into a three-register tuple.
//
// flat-carrier memory access: address formation and the bl1/bl2 behavior are part of the flat contract; the CPU model passes the data words through (wider tuples by their leading words).

u64 lo = (u64)src[0] | ((u64)src[1] << 32);
dst[0] = (u32)(lo & 0xffffffffu);
dst[1] = (u32)(lo >> 32);
dst[2] = src[2];
