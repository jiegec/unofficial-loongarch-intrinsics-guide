// s_cmp_ne_u64 -- Compares two 64-bit scalar register pairs for inequality: `d = (a != b) ? 0xffff : 0`.
//

dst[0] = cmp_ne_u64(((u64)src[0] | ((u64)src[0+1] << 32)),
                    ((u64)src[2] | ((u64)src[2+1] << 32)), policy);
