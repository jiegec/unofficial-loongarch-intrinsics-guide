// store_sm_b8 -- Stores the low 8 bits of a vector source register into workgroup shared memory as a bit pattern.
//
// [HW 2026-10-22] the model passed the data word through, but only the low byte is stored: with the
// value 0x7fffffff the hardware side reads back 0x000000ff.

dst[0] = src[0] & 0xffu;
