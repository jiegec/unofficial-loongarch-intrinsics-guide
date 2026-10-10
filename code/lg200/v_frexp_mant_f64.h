// v_frexp_mant_f64 -- Returns the normalized significand of each lane's f64 source: `d = m` with `m` in [0.5, 1).
//

u64 v = ((u64)src[0] | ((u64)src[0+1] << 32));
if ((v & 0x7ff0000000000000ull) == 0 && (v & 0x000fffffffffffffull) != 0)
  v = (v & 0x8000000000000000ull);
u64 r = frexp_mant_f64(v);
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
