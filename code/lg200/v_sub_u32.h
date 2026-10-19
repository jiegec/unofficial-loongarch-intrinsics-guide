// v_sub_u32 -- Subtracts the second source from the first source and writes the result to the vector destination
// v_sub_u32 -- vector subtract with borrow-out. dst[0] = src0 - src1,
// dst[1] = borrow-out (1 when src0 < src1).
u32 diff = src[0] - src[1];
dst[0] = diff;
dst[1] = (src[0] < src[1]) ? 1u : 0u;
