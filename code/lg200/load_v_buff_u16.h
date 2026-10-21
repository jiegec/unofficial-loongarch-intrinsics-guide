// load_v_buff_u16 -- Reads one unsigned halfword through a buffer descriptor and zero-extends it into one 32-bit vector register.
//
// [HW 2026-10-22] the model passed the loaded word through, but the hardware zero-extends: the staged
// word 0x7fffffff reads back 0x0000ffff.

dst[0] = src[0] & 0xffffu;
