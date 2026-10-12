// v_cmp_neq_f16 -- Compares two f16 values for inequality and writes a lane mask: `d = (a != b) ? 0xffff : 0x0000`.
//

dst[0] = cmp_neq_f16(src[0], src[1], policy);
