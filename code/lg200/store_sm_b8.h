// store_sm_b8 -- Stores the low 8 bits of a vector source register into workgroup shared memory as a bit pattern.
//
// lds-carrier write: the shared-memory address/permutation formation is part of the workgroup lds contract; the CPU model passes the data words through (wider tuples by their leading words).

dst[0] = src[0];
