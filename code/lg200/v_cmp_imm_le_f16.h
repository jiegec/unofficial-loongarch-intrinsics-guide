// v_cmp_imm_le_f16 -- Compares each f16 lane of `VJ8` with the immediate `IK32` and writes the lane mask to `RD8`.
//

dst[0] = cmp_le_f16(src[0], src[1], policy);
