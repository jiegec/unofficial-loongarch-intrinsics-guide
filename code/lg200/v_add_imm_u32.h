// v_add_imm_u32 -- Adds a u32 lane and a 32-bit literal, modulo 2^32.
//

u32 sum = src[0] + src[1];
dst[0] = sum;
dst[1] = (sum < src[0]) ? 1u : 0u;
