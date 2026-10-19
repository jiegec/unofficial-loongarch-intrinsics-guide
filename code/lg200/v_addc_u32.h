// v_addc_u32 -- Adds the two unsigned 32-bit integer sources and the low carry-in bit and writes the result to the vector destination
// v_addc_u32 -- vector add with carry-in; the carry lane is gated on the
// low 30 bits of VCC. dst[0] = sum, dst[1] = carry-out.
u32 low30 = src[2] & 0x3fffffffu;
u32 s = src[0] + src[1];
u32 sum = (low30 == 0) ? s
        : (low30 == 0x3fffffffu) ? s + 1u
        : (s | (s + 1u));
u64 wide = (u64)src[0] + src[1] + ((low30 != 0) ? 1u : 0u);
dst[0] = sum;
dst[1] = (wide > 0xffffffffu) ? 1u : 0u;
