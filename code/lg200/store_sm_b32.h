// store_sm_b32 -- Stores a 32-bit bit pattern from a vector source register into workgroup shared memory.
//
// lds-carrier write: the shared-memory address/permutation formation is part of the workgroup lds contract; the CPU model passes the data words through (wider tuples by their leading words).

dst[0] = src[0];
