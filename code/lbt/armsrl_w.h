// armsrl.w
// @meta category: arm
// @meta synopsis: armsrl.w rj, rk, cond
// @meta desc: Compute ARM flags from 32-bit LSR of rj. Updates N,Z,C if cond holds. V preserved.
//
// @reg read: GPR[rj], GPR[rk], ARMFLAGS (condition)
// @reg write: ARMFLAGS[N,Z,C]
//
// @flag N: COMPUTE          sign of result
// @flag Z: COMPUTE          result == 0
// @flag C: COMPUTE          carry-out
// @flag V: PRESERVE         preserved
// @cond: cond false => all writes suppressed and state preserved
//
{
    if (!arm_cond_holds(ARMFLAGS, cond))
        return;
    uint32_t lhs = (uint32_t)GPR[rj];
    uint32_t rhs = (uint32_t)GPR[rk];
    uint32_t r;
    uint32_t c = rhs & 0xff;
    if (c == 0) {
        r = lhs;
        // C preserved
    } else if (c < 32) {
        r = lhs >> c;
        ARMFLAGS.C = (lhs >> (c - 1)) & 1;
    } else if (c == 32) {
        r = 0;
        ARMFLAGS.C = (lhs >> 31) & 1;
    } else {
        r = 0;
        ARMFLAGS.C = 0;
    }
    ARMFLAGS.N = (r >> 31) & 1;
    ARMFLAGS.Z = r == 0;
    // GPR operands are unchanged.
}
