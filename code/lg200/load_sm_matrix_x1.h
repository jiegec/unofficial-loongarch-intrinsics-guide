// load_sm_matrix_x1 -- Loads 1 consecutive 32-bit matrix plane from workgroup shared memory into the vector destination tuple
// [HW-observed] lds-carrier matrix read: the matrix file is launch-state persistent and the plane/lane addressing layout is UNRESOLVED; the CPU model passes the read word through.
dst[0] = src[0];
