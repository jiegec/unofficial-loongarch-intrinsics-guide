// v_cmp_imm_ge_i16 -- Compares each 16-bit signed lane of `VJ8` with the immediate `IK32` and writes the lane mask to `RD8`.
//

dst[0] = cmp_ge_i16(src[0], src[1]);
