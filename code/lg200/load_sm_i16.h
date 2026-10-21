// load_sm_i16 -- Loads a signed 16-bit integer from workgroup shared memory and sign-extends it into a vector register.
//
// [HW 2026-10-22] the model passed the data word through, but the hardware sign-extends: the staged
// word 0x00008000 returns 0xffff8000 and 0x0000fffe returns 0xfffffffe.

u32 v = src[0] & 0xffffu;
dst[0] = (v & 0x8000u) ? (v | 0xffff0000u) : v;
