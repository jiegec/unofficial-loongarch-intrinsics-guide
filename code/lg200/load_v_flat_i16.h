// load_v_flat_i16 -- Loads a per-lane signed 16-bit flat element and sign-extends it to 32 bits.
//
// flat-carrier memory access: address formation and the bl1/bl2 behavior are part of the flat contract; the model takes the word the carrier stages at that address as the memory content and reads the element out of its low halfword.
//
// [HW 2026-11-03] the model passed the data word through, but the hardware sign-extends: with the staged word 0x00008000 the load returns 0xffff8000 and 0x0000fffe returns 0xfffffffe (compute pipe, LGPU_FLAT_COMPARE=1, gen56064/gen56065).

dst[0] = sext_i32_i16(src[0]);
