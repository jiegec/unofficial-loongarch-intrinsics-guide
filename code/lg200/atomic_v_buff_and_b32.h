// atomic_v_buff_and_b32 -- Atomically reads a 32-bit bit pattern from the addressed buffer resource, computes new = old AND source, and writes new back to the same location
// [HW-observed] buff-carrier atomic: the addressed buffer location and the RMW happen on the buffer path; the CPU model computes the updated words (old -> dst[0..], updated -> the trailing dst words).
u32 updated = atomic_v_buff_and_b32(src[0], src[1]);
dst[0] = src[0];
dst[1] = updated;
