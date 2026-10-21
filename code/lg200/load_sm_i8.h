// load_sm_i8 -- Loads a signed 8-bit integer from workgroup shared memory and sign-extends it into a vector register.
//
// [HW 2026-10-22] the model passed the data word through, but the hardware sign-extends: with the
// staged word 0x00000080 the load returns 0xffffff80, and 0x000000fe returns 0xfffffffe.  The
// extension is what the mnemonic names, so it is applied to the low byte of the passed word.

u32 v = src[0] & 0xffu;
dst[0] = (v & 0x80u) ? (v | 0xffffff00u) : v;
