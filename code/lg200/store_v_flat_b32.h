// store_v_flat_b32 -- Stores a full 32-bit source word to a per-lane flat address.
//
// flat-carrier memory access: address formation and the bl1/bl2 behavior are part of the flat contract; the CPU model passes the data words through (wider tuples by their leading words).

dst[0] = src[0];
