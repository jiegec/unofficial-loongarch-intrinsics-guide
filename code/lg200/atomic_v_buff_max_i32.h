// atomic_v_buff_max_i32 -- Atomically reads a signed 32-bit integer from the addressed buffer resource, computes new is the greater of old and source, and writes new back to the same location
// [HW-observed] buff-carrier atomic: the addressed buffer location and the RMW happen on the buffer path; the CPU model computes the updated words (old -> dst[0..], updated -> the trailing dst words).
u32 updated = atomic_v_buff_max_i32(src[0], src[1]);
dst[0] = src[0];
dst[1] = updated;
