// s_cmp_imm -- Compares the 32-bit scalar source with the encoded immediate
// for equality and writes 0xffff for true or zero for false to SCC.
// True materializes 0xffff, false 0, written to SCC; only the value result
// is modeled here.
dst[0] = cmp_eq_u32(src[0], src[1]);
