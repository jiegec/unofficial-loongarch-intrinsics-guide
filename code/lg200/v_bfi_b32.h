// v_bfi_b32 -- Inserts the selected source bits into the base value under the mask and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = (src[0] & src[1]) | (~src[0] & src[2]);
