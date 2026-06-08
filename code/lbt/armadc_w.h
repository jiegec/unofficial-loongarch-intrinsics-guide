// armadc.w
// @meta category: arm
// @meta synopsis: armadc.w rj, rk, cond
// @meta desc: Compute ARM flags from 32-bit ADC of rj, rk, and C. Updates N,Z,C,V if cond holds.
//
// @reg read: GPR[rj], GPR[rk], ARMFLAGS (condition)
// @reg write: ARMFLAGS[N,Z,C,V]
//
// @flag N: COMPUTE          sign of result
// @flag Z: COMPUTE          result == 0
// @flag C: COMPUTE          carry/borrow
// @flag V: COMPUTE          signed overflow
// @cond: cond false => all writes suppressed and state preserved
//
{
    if (!arm_cond_holds(ARMFLAGS, cond))
        return;
    uint32_t lhs = (uint32_t)GPR[rj];
    uint32_t rhs = (uint32_t)GPR[rk];
    uint32_t carry_in = ARMFLAGS.C;
    uint64_t wide = (uint64_t)lhs + rhs + carry_in;
    uint32_t r = (uint32_t)wide;
    ARMFLAGS.C = (wide >> 32) & 1;
    ARMFLAGS.V = ((~(lhs ^ rhs)) & (lhs ^ r) & 0x80000000) != 0;
    ARMFLAGS.N = (r >> 31) & 1;
    ARMFLAGS.Z = r == 0;
    // GPR operands are unchanged.
}
