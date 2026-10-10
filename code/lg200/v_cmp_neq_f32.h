// v_cmp_neq_f32 -- Compares two f32 values for inequality and writes a lane mask: `d = (a != b) ? 0xffff : 0x0000`.
//

dst[0] = cmp_neq_f32(src[0], src[1]);
