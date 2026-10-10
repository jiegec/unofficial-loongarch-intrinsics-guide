// v_fcvt_f32off_i4 -- Sign-extends the low nibble of each lane's source and scales it by 2^-4: `d = f32(i4(a[3:0])) * 2^-4`.
//

dst[0] = fcvt_f32off_i4(src[0]);
