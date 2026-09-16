// load_sm_interp -- Loads one 32-bit interpolation-workspace value selected by the scalar base and 8-bit offset into the active lane's vector destination
// [HW-observed] lds-carrier read: the shared-memory address/permutation formation is part of the workgroup lds contract; the CPU model passes the data words through (wider tuples by their leading words).
dst[0] = src[0];
