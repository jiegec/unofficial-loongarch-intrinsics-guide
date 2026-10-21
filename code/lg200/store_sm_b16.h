// store_sm_b16 -- Stores the low 16 bits of a vector source register into workgroup shared memory as a bit pattern.
//
// [HW 2026-10-22] the model passed the data word through, but only the low halfword is stored.

dst[0] = src[0] & 0xffffu;
