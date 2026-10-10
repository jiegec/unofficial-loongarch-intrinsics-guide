// v_sub_u32 -- Subtracts the second u32 source from the first, writing a difference and a borrow-out.
//

u32 diff = src[0] - src[1];
dst[0] = diff;
dst[1] = (src[0] < src[1]) ? 1u : 0u;
