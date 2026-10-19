// v_mma_323216_u8_u8_i32 -- Performs the 323216 packed matrix multiply-accumulate using unsigned 8-bit integer and unsigned 8-bit integer elements with a signed 32-bit integer accumulator
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = v_mma(src[0], src[1], src[2], false);
