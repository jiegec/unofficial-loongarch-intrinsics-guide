// Atomically reads an unsigned 32-bit integer from the addressed flat-memory location, computes new is the greater of old and source, and writes new back to the same location. The destination receives old.
// [HW-observed] flat-carrier atomic: the addressed flat-memory location and the RMW happen on the flat path; the CPU model computes the updated words (old -> dst[0..], updated -> the trailing dst words) with the named atomic operation.
u32 updated = v_flat_max_u32(src[0], src[1]);
dst[0] = src[0];
dst[1] = updated;
