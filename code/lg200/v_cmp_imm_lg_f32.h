// v_cmp_imm_lg_f32 -- Compares each f32 lane of `VJ8` with the immediate `IK32` and writes the lane mask to `RD8`.
//

dst[0] = cmp_lg_f32(src[0], src[1]);
