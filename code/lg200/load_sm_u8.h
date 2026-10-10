// load_sm_u8 -- Loads an unsigned 8-bit integer from workgroup shared memory, zero-extended into a per-lane vector destination register.
//
// lds-carrier read: the shared-memory address/permutation formation is part of the workgroup lds contract; the CPU model passes the data words through (wider tuples by their leading words).

dst[0] = src[0];
