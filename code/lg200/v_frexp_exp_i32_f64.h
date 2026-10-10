// v_frexp_exp_i32_f64 -- Returns the unbiased exponent of each lane's f64 source as a signed 32-bit integer: `d = e`.
//

u64 v = ((u64)src[0] | ((u64)src[0+1] << 32));
if ((v & 0x7ff0000000000000ull) == 0 && (v & 0x000fffffffffffffull) != 0)
  v = (v & 0x8000000000000000ull);
dst[0] = frexp_exp_i32_f64(v);
