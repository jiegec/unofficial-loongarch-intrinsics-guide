// v_fcvt_i32flr_f32 -- Converts each lane's single-precision source to a signed 32-bit integer, rounding toward -infinity: `d = floor(a)`.
//

dst[0] = fcvt_i32flr_f32(src[0]);
