// load_sm_matrix_x2 -- Loads two consecutive 32-bit matrix planes from the launch-persistent workspace into a two-register tuple.
//
// lds-carrier matrix read: the matrix file is launch-state persistent and the plane/lane addressing layout is UNRESOLVED; the CPU model passes the read word through.

u64 r = ((u64)src[0] | ((u64)src[0+1] << 32));
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
