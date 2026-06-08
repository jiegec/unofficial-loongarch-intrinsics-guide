// addu12i.d
// @meta category: base_integer
// @meta synopsis: addu12i.d rd, rj, simm5
// @meta desc: Add 64-bit `rj` and sign-extended `(simm5 << 12)`. Store the result in `rd`. Flags unchanged.
//
// @reg read: GPR[rj]
// @reg write: GPR[rd]
// @reg preserve: GPR[rj], EFLAGS
//
// @flag: none
// @cond: none
//
{
    uint64_t a = (uint64_t)GPR[rj];
    int64_t offset = (int64_t)sext(imm & 0x1f, 5) << 12;
    uint64_t r = a + (uint64_t)offset;
    GPR[rd] = r;
    // All flags unchanged.
}
