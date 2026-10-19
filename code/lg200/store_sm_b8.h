// store_sm_b8 -- Stores a 8-bit bit pattern from the vector source to workgroup shared memory
// [HW-observed] lds-carrier write: the shared-memory address/permutation formation is part of the workgroup lds contract; the CPU model passes the data words through (wider tuples by their leading words).
dst[0] = src[0];
