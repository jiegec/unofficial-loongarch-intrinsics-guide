// v_dp4a_i8_u8_i32 -- Accumulates a dot product of packed signed bytes with packed unsigned bytes: `d = acc + sum(a_i * b_i)`.
//

dst[0] = dp4a_i8_u8_i32(src[0], src[1]);
