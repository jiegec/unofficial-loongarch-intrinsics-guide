// v_sin_f32 -- Computes the sine of an angle given as a fraction of a full turn: `d = sin(2*pi*a)`.
//
// [HW 2026-10-22] two rules measured on this row.  A NaN operand is QUIETED, keeping its sign
// and payload - 0x7f800001 returns 0x7fc00001 and 0xff800001 returns 0xffc00001 - which is the
// rsq/log2/sqrt behaviour rather than frexp's passthrough.  And a zero result takes the sign of
// the input, as an odd function must: -3.4e38 returns 0x80000000, where the oracle gave +0.

u32 v = src[0];
if ((v & 0x7f800000u) == 0x7f800000u && (v & 0x007fffffu) != 0u) {
  dst[0] = v | 0x00400000u;
} else {
  u32 r = f32_sin(v, policy);
  if ((r & 0x7fffffffu) == 0u) r = v & 0x80000000u;
  dst[0] = r;
}
