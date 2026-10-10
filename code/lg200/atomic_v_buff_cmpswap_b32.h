// atomic_v_buff_cmpswap_b32 -- Atomically swaps a 32-bit replacement into the addressed buffer element on a compare match, returning the old contents.
//
// buff-carrier atomic: the addressed buffer location and the RMW happen on the buffer path; the CPU model computes the updated words (old -> dst[0..], updated -> the trailing dst words).

u32 updated = v_buff_cmpswap_b32(src[0], src[1], src[2]);
dst[0] = src[0];
dst[1] = updated;
