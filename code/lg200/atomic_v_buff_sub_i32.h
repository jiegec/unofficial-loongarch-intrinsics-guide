// atomic_v_buff_sub_i32 -- Atomically subtracts a 32-bit source from a buffer location: `new = old - src`.
//
// buff-carrier atomic: the addressed buffer location and the RMW happen on the buffer path; the CPU model computes the updated words (old -> dst[0..], updated -> the trailing dst words).

u32 updated = v_buff_sub_i32(src[0], src[1]);
dst[0] = src[0];
dst[1] = updated;
