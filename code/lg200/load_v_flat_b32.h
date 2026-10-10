// load_v_flat_b32 -- Loads a per-lane 32-bit element from a flat address into one vector register.
//
// flat-carrier memory access: address formation and the bl1/bl2 behavior are part of the flat contract; the CPU model passes the data words through (wider tuples by their leading words).

dst[0] = src[0];
