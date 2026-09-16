// store_sm_b64 -- Stores a 64-bit bit pattern from the vector source to workgroup shared memory
// [HW-observed] lds-carrier write: the shared-memory address/permutation formation is part of the workgroup lds contract; the CPU model passes the data words through (wider tuples by their leading words).
u64 r = ((u64)src[0] | ((u64)src[0+1] << 32));
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
