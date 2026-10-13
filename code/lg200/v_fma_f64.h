// v_fma_f64 -- Computes a fused multiply-add of three f64 sources: `d = a * b + c`.
//
// [HW 2026-10-13] this row does not honour the rounding mode.  Its rtz, rtup and rtdn cases
// all return the value the rtne case returns, where v_add_f64 and the f32 fma do follow the
// mode - so the policy is fixed here and the operand and result flushes mirror v_fma_f32.

auto ftz = [](u64 v) -> u64 {
  return (v & 0x7ff0000000000000ull) == 0ull ? (v & 0x8000000000000000ull) : v;
};
u64 a = ftz(((u64)src[0] | ((u64)src[0+1] << 32)));
u64 b = ftz(((u64)src[2] | ((u64)src[2+1] << 32)));
u64 c = ftz(((u64)src[4] | ((u64)src[4+1] << 32)));
test::FpPolicy hw{};
hw.daz = true;
hw.ftz = true;
u64 r = f64_fma(a, b, c, hw);
r = ftz(r);
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
