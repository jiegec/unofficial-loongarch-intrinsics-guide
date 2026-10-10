// v_addc_u32 -- Adds two u32 values plus a carry-in: `d = a + b + cin`, with carry-out.
//

u32 low30 = src[2] & 0x3fffffffu;
u32 s = src[0] + src[1];
u32 sum = (low30 == 0) ? s
        : (low30 == 0x3fffffffu) ? s + 1u
        : (s | (s + 1u));
u64 wide = (u64)src[0] + src[1] + ((low30 != 0) ? 1u : 0u);
dst[0] = sum;
dst[1] = (wide > 0xffffffffu) ? 1u : 0u;
