// v_cmpclass_f64 -- Tests each f64 lane for membership in a class set: `d = (class(a) & m) != 0`.
//

u64 r = cmpclass_f64(((u64)src[2] | ((u64)src[2+1] << 32)), ((u64)src[0] | ((u64)src[0+1] << 32)));
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
