// v_add_u32 -- Adds two u32 values: `d = a + b` (mod 2^32), plus the carry-out.
//

u32 lhs = src[0];
u32 rhs = src[1];
u32 sum = lhs + rhs;
dst[0] = sum;
dst[1] = (sum < lhs) ? 1u : 0u;
