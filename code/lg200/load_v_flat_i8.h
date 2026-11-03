// load_v_flat_i8 -- Loads a per-lane signed 8-bit flat element and sign-extends it to 32 bits.
//
// flat-carrier memory access: address formation and the bl1/bl2 behavior are part of the flat contract; the model takes the word the carrier stages at that address as the memory content and reads the element out of its low byte.
//
// [HW 2026-11-03] the model passed the data word through, but the hardware sign-extends: with the staged word 0x00000080 the load returns 0xffffff80 and 0x000000fe returns 0xfffffffe (compute pipe, LGPU_FLAT_COMPARE=1, gen56074/gen56075).  The extension is what the mnemonic names, so it is applied to the low byte of the passed word - the same measurement load_sm_i8 records.

dst[0] = sext_i32_i8(src[0]);
