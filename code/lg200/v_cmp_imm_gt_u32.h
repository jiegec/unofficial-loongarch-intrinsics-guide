// v_cmp_imm_gt_u32 -- Compares each 32-bit unsigned lane of `VJ8` with the immediate `IK32` and writes the lane mask to `RD8`.
//

dst[0] = cmp_gt_u32(src[0], src[1]);
