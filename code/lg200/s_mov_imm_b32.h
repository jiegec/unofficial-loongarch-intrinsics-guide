// s_mov_imm_b32 -- Copies the 32-bit source value and writes the result to
// the scalar destination.
// The scalar ALU form also updates SCC as a side effect; only the value
// result is modeled here.
// The encoded immediate supplies the value; src[0] carries the
// appropriately sign/zero-extended load on this carrier.
dst[0] = src[0];
