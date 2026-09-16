// v_pack_i16_i32 -- Packs source values from signed 32-bit integer representation into signed 16-bit integer fields in the 32-bit vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = pack_i16_i32(src[0], src[1]);
