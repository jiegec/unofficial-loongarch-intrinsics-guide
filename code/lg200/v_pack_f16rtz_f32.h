// v_pack_f16rtz_f32 -- Packs source values from 32-bit floating-point value representation into f16rtz fields in the 32-bit vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = v_pack_f16rtz_f32(src[0], src[1]);
