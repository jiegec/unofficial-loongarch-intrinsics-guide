// v_fract_f64 -- Computes the fractional part of each lane's f64 source: `d = a - floor(a)`.
//
// [HW 2026-10-22] three rules, mirroring the f32 sibling.  A denormal input is flushed to
// signed zero before the subtraction; a NaN is transmitted with sign and payload, quieted if
// signaling; and an INFINITY returns zero rather than the NaN that inf - floor(inf) makes -
// the row's inf-pos and inf-neg cases both expect 0.

u64 v = ((u64)src[0] | ((u64)src[1] << 32));
v = (v & 0x7ff0000000000000ull) == 0ull ? (v & 0x8000000000000000ull) : v;
const u64 mag = v & 0x7ff0000000000000ull;
u64 r;
if (mag == 0x7ff0000000000000ull && (v & 0x000fffffffffffffull) != 0ull) {
  r = v | 0x0008000000000000ull;  // a NaN operand is quieted, sign and payload kept
} else if (mag == 0x7ff0000000000000ull) {
  r = 0ull;  // an infinity has no fractional part
} else {
  r = f64_fract(v, policy);
  if ((r & 0x7fffffffffffffffull) == 0ull) r = 0ull;
}
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
