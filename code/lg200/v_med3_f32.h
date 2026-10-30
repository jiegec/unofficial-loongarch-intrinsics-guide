// v_med3_f32 -- Selects the middle of three f32 values: `d = med(a, b, c)`.
//
// NaN: a NaN input is quieted and the row returns that NaN - it does not sort
// it to a bound.  Among NaNs a signaling one outranks a quiet one wherever it
// sits, and among the same kind the caller's operand order decides.  That rule
// comes from the fifteen-triple hardware probe recorded in ops/softfloat.cpp
// sf_med3(), and the case vectors pin four of those triples.
//
// Subnormal inputs are flushed to +-0 before the comparison, exactly as
// v_min_f32 does (its subnormal-neg case reads -0.0 for min(-2^-149, 1.0)), so a
// clamp built from med3 and one built from a min/max pair agree on denormals.
//
u32 a0 = (src[0] & 0x7f800000u) == 0u ? (src[0] & 0x80000000u) : src[0];
u32 a1 = (src[1] & 0x7f800000u) == 0u ? (src[1] & 0x80000000u) : src[1];
u32 a2 = (src[2] & 0x7f800000u) == 0u ? (src[2] & 0x80000000u) : src[2];
dst[0] = f32_med3(a0, a1, a2, policy);
