// atomic_v_buff_cmpswap_b32 -- Atomically reads a 32-bit bit pattern from the addressed buffer resource, computes new = swap when old equals compare, otherwise old, and writes new back to the same location
// [HW-observed] buff-carrier atomic: the addressed buffer location and the RMW happen on the buffer path; the CPU model computes the updated words (old -> dst[0..], updated -> the trailing dst words).
u32 updated = v_buff_cmpswap_b32(src[0], src[1], src[2]);
dst[0] = src[0];
dst[1] = updated;
