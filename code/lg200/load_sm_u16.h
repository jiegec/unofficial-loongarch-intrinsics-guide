// load_sm_u16 -- Loads an unsigned 16-bit integer from workgroup shared memory, zero-extended into a per-lane vector register.
//
// [HW 2026-10-22] the model passed the data word through, but the hardware zero-extends: only the
// low halfword of the staged word survives.

dst[0] = src[0] & 0xffffu;
