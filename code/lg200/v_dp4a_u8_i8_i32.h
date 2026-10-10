// v_dp4a_u8_i8_i32 -- Accumulates a dot product of packed unsigned bytes with packed signed bytes: `d = acc + sum(a_i * b_i)`.
//

dst[0] = dp4a_u8_i8_i32(src[0], src[1]);
