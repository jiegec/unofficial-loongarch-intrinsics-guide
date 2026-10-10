// v_bfe_u32 -- Extracts a bit field from a 32-bit value and zero-extends it: `d = zext(a[off+width-1:off])`.
//

dst[0] = bfe_u32(src[0], src[1], src[2]);
