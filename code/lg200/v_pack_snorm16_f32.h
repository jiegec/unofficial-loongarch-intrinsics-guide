// v_pack_snorm16_f32 -- Packs source values from 32-bit floating-point value representation into signed normalized 16-bit value fields in the 32-bit vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = pack_snorm16_f32(src[0]);
