// load_sm_b32 -- Loads a 32-bit bit pattern from workgroup shared memory into the vector destination
// [HW-observed] lds-carrier read: the shared-memory address/permutation formation is part of the workgroup lds contract; the CPU model passes the data words through (wider tuples by their leading words).
dst[0] = src[0];
