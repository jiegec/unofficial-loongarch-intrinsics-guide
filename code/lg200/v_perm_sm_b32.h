// v_perm_sm_b32 -- Permutes source bytes using the workgroup shared-memory permutation state and writes the result to the vector destination
// [HW-observed] lds-carrier sm-perm: the shared-memory address/permutation formation is part of the workgroup lds contract; the CPU model passes the data words through (wider tuples by their leading words).
dst[0] = src[0];
