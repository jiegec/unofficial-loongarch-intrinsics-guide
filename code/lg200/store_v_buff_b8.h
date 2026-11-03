// store_v_buff_b8 -- Writes the low byte of one vector register to a descriptor-addressed buffer location.
//
// buff-carrier store: the descriptor/address formation is part of the buffer contract; the model states what the narrow store leaves in the dword.
//
// [HW 2026-11-03] Only the low byte is written - a 0x12345678 datum reads back
// 0x00000078 on both pipes - the same narrowing the flat sibling and the buffer
// b16 form already carry.  It was observed rather than compared because the
// model was the b32 identity, so the readback looked like a model failure.

dst[0] = src[0] & 0xffu;
