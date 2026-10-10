// v_fcvt_i32rpi_f32 -- Converts each lane's single-precision source to a signed 32-bit integer, rounding toward +infinity: `d = ceil(a)`.
//

u32 v = (src[0] & 0x7f800000u) == 0u ? 0u : src[0];
dst[0] = fcvt_i32rpi_f32(v);
