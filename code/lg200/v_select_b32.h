// v_select_b32 -- Selects one of two 32-bit lane values under a per-lane condition: `d = c ? A : b`.
//

dst[0] = select_b32(src[0], src[1], src[2]);
