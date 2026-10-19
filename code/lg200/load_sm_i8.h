// load_sm_i8 -- Loads a signed 8-bit integer from workgroup shared memory into the vector destination and sign-extends it to 32 bits
// [HW-observed] lds-carrier read: the shared-memory address/permutation formation is part of the workgroup lds contract; the CPU model passes the data words through (wider tuples by their leading words).
dst[0] = src[0];
