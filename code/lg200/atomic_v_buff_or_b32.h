// atomic_v_buff_or_b32 -- Atomically ORs a 32-bit source pattern into the addressed buffer element and returns its old contents.
//
// buff-carrier atomic: the addressed buffer location and the RMW happen on the buffer path; the CPU model computes the updated words (old -> dst[0..], updated -> the trailing dst words).

u32 updated = atomic_v_buff_or_b32(src[0], src[1]);
dst[0] = src[0];
dst[1] = updated;
