// load_sm_u16 -- Loads an unsigned 16-bit integer from workgroup shared memory, zero-extended into a per-lane vector register.
//
// lds-carrier read: the shared-memory address/permutation formation is part of the workgroup lds contract; the CPU model passes the data words through (wider tuples by their leading words).

dst[0] = src[0];
