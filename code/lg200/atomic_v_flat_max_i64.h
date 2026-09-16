// Atomically reads a signed 64-bit integer from the addressed flat-memory location, computes new is the greater of old and source, and writes new back to the same location. The destination receives old.
// [HW-observed] flat-carrier atomic: the addressed flat-memory location and the RMW happen on the flat path; the CPU model computes the updated words (old -> dst[0..], updated -> the trailing dst words) with the named atomic operation.
u64 updated = v_flat_max_i64(((u64)src[0] | ((u64)src[0+1] << 32)), ((u64)src[2] | ((u64)src[2+1] << 32)));
dst[0] = src[0];
dst[1] = src[1];
dst[2] = (u32)(updated & 0xffffffffu);
dst[3] = (u32)(updated >> 32);
