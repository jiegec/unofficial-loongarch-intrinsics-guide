// v_add_u32 -- Adds the two unsigned 32-bit integer sources and writes the result to the vector destination
// dst[0] = src0 + src1 (wraps at 32 bits); dst[1] = carry-out.
u32 lhs = src[0];
u32 rhs = src[1];
u32 sum = lhs + rhs;
dst[0] = sum;
dst[1] = (sum < lhs) ? 1u : 0u;
