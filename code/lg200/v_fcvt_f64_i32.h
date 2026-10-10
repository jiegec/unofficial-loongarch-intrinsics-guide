// v_fcvt_f64_i32 -- Converts each lane's signed 32-bit integer source to double precision: `d = f64(i32(a))`.
//

u64 r = i32_f64(src[0]);
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
