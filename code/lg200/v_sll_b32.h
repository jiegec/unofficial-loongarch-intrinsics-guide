// v_sll_b32 -- Shifts a 32-bit lane value left by a per-lane count: `d = a << (b & 31)`.
//

dst[0] = src[0] << (src[1] & 31u);
