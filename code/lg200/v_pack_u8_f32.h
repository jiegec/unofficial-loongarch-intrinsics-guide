// v_pack_u8_f32 -- Packs source values from 32-bit floating-point value representation into unsigned 8-bit integer fields in the 32-bit vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = pack_u8_f32(src[0], src[1], src[2]);
