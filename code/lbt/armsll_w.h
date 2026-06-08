// armsll.w
// @meta category: arm
// @meta synopsis: armsll.w rj, rk, cond
// @meta desc: Compute ARM flags from 32-bit LSL of rj. Updates N,Z,C if cond holds. V preserved.
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
    uint32_t c = (uint32_t)GPR[rk] & 0xff;
    uint32_t r;

    if (c == 0) {
        // C preserved, only N/Z updated from unshifted value.
        r = lhs;
        ARMFLAGS.N = (r >> 31) & 1;
        ARMFLAGS.Z = r == 0;
        return;
    }

    if (c < 32) {
        ARMFLAGS.C = (lhs >> (32 - c)) & 1;
        r = lhs << c;
    } else if (c == 32) {
        ARMFLAGS.C = lhs & 1;
        r = 0;
    } else {
        ARMFLAGS.C = 0;
        r = 0;
    }
    ARMFLAGS.N = (r >> 31) & 1;
    ARMFLAGS.Z = r == 0;
}
