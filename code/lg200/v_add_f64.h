// v_add_f64 -- Adds two f64 values held in register pairs: `d = a + b`.
//

test::FpPolicy hw{};
hw.daz = true;
hw.ftz = true;
u64 r = f64_add(((u64)src[0] | ((u64)src[0+1] << 32)), ((u64)src[2] | ((u64)src[2+1] << 32)), hw);
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
