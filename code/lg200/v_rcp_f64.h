// v_rcp_f64 -- Produces an approximation of the reciprocal of an f64 value as a two-register result: `d = 1/a`.
//
// [HW 2026-10-13] the same two flushes its f32 sibling carries and this model lacked: a
// denormal OPERAND becomes signed zero first, so rcp(0x8000000000000001) returns -inf, and
// a denormal RESULT flushes to signed zero, so rcp(0x7fefffffffffffff) returns 0 where the
// exact reciprocal is 0x00040000.

u64 v = ((u64)src[0] | ((u64)src[1] << 32));
v = (v & 0x7ff0000000000000ull) == 0ull ? (v & 0x8000000000000000ull) : v;
u64 r = f64_rcp(v, policy);
if ((r & 0x7ff0000000000000ull) == 0ull && (r & 0x000fffffffffffffull) != 0ull)
  r &= 0x8000000000000000ull;
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
