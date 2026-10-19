// v_fcvt_i32rpi_f32 -- Converts the 32-bit floating-point source to signed 32-bit integer with round-toward-positive-infinity and writes the result to the vector destination
// [HW 2026-08-24] denormal inputs flush to +0 before the conversion.
u32 v = (src[0] & 0x7f800000u) == 0u ? 0u : src[0];
dst[0] = fcvt_i32rpi_f32(v);
