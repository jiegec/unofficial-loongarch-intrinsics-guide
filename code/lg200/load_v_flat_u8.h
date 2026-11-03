// load_v_flat_u8 -- Loads a per-lane unsigned 8-bit flat element and zero-extends it to 32 bits.
//
// flat-carrier memory access: address formation and the bl1/bl2 behavior are part of the flat contract; the model takes the word the carrier stages at that address as the memory content and reads the element out of its low byte.
//
// [HW 2026-11-03] the model passed the data word through, but the hardware zero-extends: the staged word 0x7fffffff returns 0x000000ff and 0x00000080 returns 0x00000080 (compute pipe, LGPU_FLAT_COMPARE=1, gen56097/gen56094), so only the low byte survives.

dst[0] = src[0] & 0xffu;
