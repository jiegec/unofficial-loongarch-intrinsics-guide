// load_sm_matrix_x4 -- Loads four consecutive 32-bit matrix planes into a four-register vector tuple.
//
// lds-carrier matrix read: the matrix file is launch-state persistent and the plane/lane addressing layout is UNRESOLVED; the model passes the four read words through.
//
// [HW 2026-11-03] This body used to be the x2 page's text copied verbatim, so it
// declared two result words while the encoding's tuple is VD8x4 and the case
// vectors carried two.  Two of the four planes were therefore never asserted -
// the carrier staged four inputs and read four dwords, and the comparator walked
// the two the model named.  It states all four now, and the vectors carry them.

dst[0] = src[0];
dst[1] = src[1];
dst[2] = src[2];
dst[3] = src[3];
