// load_v_flat_u16 -- Loads a per-lane unsigned 16-bit flat element and zero-extends it to 32 bits.
//
// flat-carrier memory access: address formation and the bl1/bl2 behavior are part of the flat contract; the model takes the word the carrier stages at that address as the memory content and reads the element out of its low halfword.
//
// [HW 2026-11-03] the model passed the data word through, but the hardware zero-extends: the staged word 0x7fffffff returns 0x0000ffff (compute pipe, LGPU_FLAT_COMPARE=1, gen56087), so only the low halfword survives.

dst[0] = src[0] & 0xffffu;
