// v_cmp_imm_ge_u16 -- Compares each 16-bit unsigned lane of `VJ8` with the immediate `IK32` and writes the lane mask to `RD8`.
//

dst[0] = cmp_ge_u16(src[0], src[1]);
