// v_sub_f32 -- Subtracts the second source from the first source and writes the result to the vector destination
// The PS carrier cannot program FP rounding or flush mode: the unit runs in a
// fixed hardware default (round-to-nearest-even, denormals flushed on input
// and output, derived NaN = positive 0x7FC00000). The policy's rounding and
// flush fields are reduced to that default below.
test::FpPolicy hw{};
hw.daz = true;
hw.ftz = true;
dst[0] = f32_sub(src[0], src[1], hw);
