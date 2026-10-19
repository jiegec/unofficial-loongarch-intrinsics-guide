// load_sm_swizzle_b64 -- Loads and applies the shared-memory swizzle mapping to a 64-bit value, writing the two-dword vector destination
// [HW-observed] lds-carrier read: the shared-memory address/permutation formation is part of the workgroup lds contract; the CPU model passes the data words through (wider tuples by their leading words).
u64 r = ((u64)src[0] | ((u64)src[0+1] << 32));
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
