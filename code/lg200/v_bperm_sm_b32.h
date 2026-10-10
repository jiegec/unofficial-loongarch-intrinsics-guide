// v_bperm_sm_b32 -- Permutes the bytes of a 32-bit lane value through the workgroup shared-memory path: `d = perm(a)`.
//
// lds-carrier sm-perm: the shared-memory address/permutation formation is part of the workgroup lds contract; the CPU model passes the data words through (wider tuples by their leading words).

dst[0] = src[0];
