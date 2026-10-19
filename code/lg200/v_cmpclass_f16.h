// v_cmpclass_f16 -- Tests the 16-bit floating-point value source against the encoded floating-point class mask and writes the per-lane Boolean mask to the scalar destination
// The class mask is encoded in the source field; only the per-lane Boolean mask value is modeled here.
dst[0] = cmpclass_f16(src[1], src[0]);
