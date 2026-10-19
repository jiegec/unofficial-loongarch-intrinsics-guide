// v_cmpclass_f64 -- Tests the 64-bit floating-point value source against the encoded floating-point class mask and writes the per-lane Boolean mask to the scalar destination
// The class mask is encoded in the source field; only the per-lane Boolean mask value is modeled here.
u64 r = cmpclass_f64(((u64)src[2] | ((u64)src[2+1] << 32)), ((u64)src[0] | ((u64)src[0+1] << 32)));
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
