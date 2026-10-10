// load_sm_matrix_x1 -- Loads one 32-bit matrix plane from the launch-persistent matrix workspace into a vector register.
//
// lds-carrier matrix read: the matrix file is launch-state persistent and the plane/lane addressing layout is UNRESOLVED; the CPU model passes the read word through.

dst[0] = src[0];
