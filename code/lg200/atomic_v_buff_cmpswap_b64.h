// atomic_v_buff_cmpswap_b64 -- Atomically reads a 64-bit bit pattern from the addressed buffer resource, computes new = swap when old equals compare, otherwise old, and writes new back to the same location
// [HW-observed] buff-carrier atomic: the addressed buffer location and the RMW happen on the buffer path; the CPU model computes the updated words (old -> dst[0..], updated -> the trailing dst words).
u64 updated = v_buff_cmpswap_b64(((u64)src[0] | ((u64)src[0+1] << 32)), ((u64)src[2] | ((u64)src[2+1] << 32)), ((u64)src[4] | ((u64)src[4+1] << 32)));
dst[0] = src[0];
dst[1] = src[1];
dst[2] = (u32)(updated & 0xffffffffu);
dst[3] = (u32)(updated >> 32);
