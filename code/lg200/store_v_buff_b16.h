// store_v_buff_b16 -- Writes the low halfword of one vector register to a descriptor-addressed buffer location.
//
// [HW 2026-10-22] the model passed the stored word through, but only the low halfword is written.

dst[0] = src[0] & 0xffffu;
