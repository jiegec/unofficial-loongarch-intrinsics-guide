// v_fcvt_f64_u32 -- Converts each lane's unsigned 32-bit integer source to double precision: `d = f64(u32(a))`.
//

u64 r = u32_f64(src[0]);
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
