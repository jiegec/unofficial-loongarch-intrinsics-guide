// v_ceil_f64 -- Rounds an f64 value held in a register pair toward positive infinity: `d = ceil_f64(a)`.
//

u64 v = ((u64)src[0] | ((u64)src[0+1] << 32));
if ((v & 0x7ff0000000000000ull) == 0 && (v & 0x000fffffffffffffull) != 0 &&
    (v & 0x8000000000000000ull) == 0)
  v = 0;
u64 r = f64_ceil(v, policy);
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
