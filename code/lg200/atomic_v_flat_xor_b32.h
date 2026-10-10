// atomic_v_flat_xor_b32 -- Atomically exclusive-ORs the flat-addressed word with a register word: `new = old XOR source`.
//
// flat-carrier atomic: the addressed flat-memory location and the RMW happen on the flat path; the CPU model computes the updated words (old -> dst[0..], updated -> the trailing dst words) with the named atomic operation.

u32 updated = v_flat_xor_b32(src[0], src[1]);
dst[0] = src[0];
dst[1] = updated;
