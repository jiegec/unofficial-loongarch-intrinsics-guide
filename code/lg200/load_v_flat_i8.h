// load_v_flat_i8 -- Loads a per-lane signed 8-bit flat element and sign-extends it to 32 bits.
//
// flat-carrier memory access: address formation and the bl1/bl2 behavior are part of the flat contract; the CPU model passes the data words through (wider tuples by their leading words).

dst[0] = src[0];
