// load_sm_u8 -- Loads an unsigned 8-bit integer from workgroup shared memory, zero-extended into a per-lane vector destination register.
//
// [HW 2026-10-22] the model passed the data word through, but the hardware zero-extends: the staged
// word 0x7fffffff returns 0x000000ff, so only the low byte survives.

dst[0] = src[0] & 0xffu;
