// addu12i.w
// @meta category: base_integer
// @meta synopsis: addu12i.w rd, rj, simm5
// @meta desc: Add 32-bit `rj` and sign-extended `(simm5 << 12)`. Store the result in `rd`. Flags unchanged.
//
// @reg read: GPR[rj]
// @reg write: GPR[rd]
// @reg preserve: GPR[rj], EFLAGS
//
// @flag: none
// @cond: none
//
{
    uint32_t a = (uint32_t)GPR[rj];
    int64_t offset = (int64_t)sext(imm & 0x1f, 5) << 12;
    uint32_t r = a + (uint32_t)offset;
    GPR[rd] = (int32_t)r;
    // All flags unchanged.
}
