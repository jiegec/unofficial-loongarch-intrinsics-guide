// v_cmp_imm_le_i32 -- Compares each 32-bit signed lane of `VJ8` with the immediate `IK32` and writes the lane mask to `RD8`.
//

dst[0] = cmp_le_i32(src[0], src[1]);
