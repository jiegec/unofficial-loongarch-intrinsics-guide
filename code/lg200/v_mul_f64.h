// v_mul_f64 -- Multiplies two f64 values: `d = a * b`.
//
// [HW 2026-10-13] the flushes are unconditional here, as in its f32 sibling: multiplying
// a denormal by 1.0 comes back zero under a bare rtne token as well as a combined one,
// so the case's policy cannot be what drives it.

test::FpPolicy hw{};
hw.daz = true;
hw.ftz = true;
u64 r = f64_mul(((u64)src[0] | ((u64)src[0+1] << 32)), ((u64)src[2] | ((u64)src[2+1] << 32)), hw);
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
