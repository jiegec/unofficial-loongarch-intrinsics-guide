// atomic_v_flat_swap_b32 -- Atomically replaces the flat-addressed word with a register word and returns the previous word.
//
// flat-carrier atomic: the addressed flat-memory location and the RMW happen on the flat path; the CPU model computes the updated words (old -> dst[0..], updated -> the trailing dst words) with the named atomic operation.

u32 updated = v_flat_swap_b32(src[0], src[1]);
dst[0] = src[0];
dst[1] = updated;
