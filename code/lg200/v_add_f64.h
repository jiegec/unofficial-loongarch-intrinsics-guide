// v_add_f64 -- Adds the two 64-bit floating-point sources and writes the result to the vector destination
// The PS carrier cannot program FP rounding or flush mode: the unit runs in a
// fixed hardware default (round-to-nearest-even, denormals flushed on input
// and output). The policy's rounding and flush fields are reduced to that
// default below.
test::FpPolicy hw{};
hw.daz = true;
hw.ftz = true;
u64 r = f64_add(((u64)src[0] | ((u64)src[0+1] << 32)), ((u64)src[2] | ((u64)src[2+1] << 32)), hw);
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
