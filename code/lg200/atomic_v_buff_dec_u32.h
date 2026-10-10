// atomic_v_buff_dec_u32 -- Atomically decrements a bounded 32-bit counter in the addressed buffer element and returns its old contents.
//
// buff-carrier atomic: the addressed buffer location and the RMW happen on the buffer path; the CPU model computes the updated words (old -> dst[0..], updated -> the trailing dst words).

u32 updated = v_buff_dec_u32(src[0], src[1]);
dst[0] = src[0];
dst[1] = updated;
