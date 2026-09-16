// v_firstv2s_b32 -- Selects the first active lane, copies its 32-bit vector source value to the scalar destination, and leaves inactive lanes without a scalar write
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = src[0];
