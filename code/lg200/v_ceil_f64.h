// v_ceil_f64 -- Rounds the 64-bit floating-point source toward positive infinity to an integral value and writes the result to the vector destination
// [HW 2026-08-24] Positive denormal inputs flush to +0.0 before ceil
// (the hardware ceil of a flushed denormal is 0, not 1.0); negative
// denormals keep -0.0.
u64 v = ((u64)src[0] | ((u64)src[0+1] << 32));
if ((v & 0x7ff0000000000000ull) == 0 && (v & 0x000fffffffffffffull) != 0 &&
    (v & 0x8000000000000000ull) == 0)
  v = 0;
u64 r = f64_ceil(v, policy);
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
