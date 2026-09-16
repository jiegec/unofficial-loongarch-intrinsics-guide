// atomic_v_buff_inc_u64 -- Atomically reads an unsigned 64-bit integer from the addressed buffer resource, computes new = old + 1 when old is below the bound, otherwise zero, and writes new back to the same location
// [HW-observed] buff-carrier atomic: the addressed buffer location and the RMW happen on the buffer path; the CPU model computes the updated words (old -> dst[0..], updated -> the trailing dst words).
u64 updated = v_buff_inc_u64(((u64)src[0] | ((u64)src[0+1] << 32)), ((u64)src[2] | ((u64)src[2+1] << 32)));
dst[0] = src[0];
dst[1] = src[1];
dst[2] = (u32)(updated & 0xffffffffu);
dst[3] = (u32)(updated >> 32);
