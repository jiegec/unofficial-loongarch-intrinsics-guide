// v_add_f32 -- Adds the two 32-bit floating-point sources and writes the result to the vector destination
// The PS carrier cannot program FP rounding or flush mode: the unit runs in a
// fixed hardware default (round-to-nearest-even, denormals flushed on input
// and output, derived NaN = positive 0x7FC00000). The policy's rounding and
// flush fields are reduced to that default below.
test::FpPolicy hw{};
hw.daz = true;
hw.ftz = true;
dst[0] = f32_add(src[0], src[1], hw);
