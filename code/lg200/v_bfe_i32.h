// v_bfe_i32 -- Extracts a bit field and sign-extends it: `d = sext(a[off+width-1:off])`.
//

dst[0] = bfe_i32(src[0], src[1], src[2]);
