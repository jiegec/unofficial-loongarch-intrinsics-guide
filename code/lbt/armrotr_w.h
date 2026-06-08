// armrotr.w
// @meta category: arm
// @meta synopsis: armrotr.w rj, rk, cond
// @meta desc: Compute ARM flags from 32-bit ROR of rj. Updates N,Z,C if cond holds. V preserved.
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
    uint32_t c;
    {
        uint32_t raw_count = rhs & 0xff;
        c = rhs & 0x1f;
        unsigned n = c % 32;
        r = n == 0 ? lhs : (lhs >> n) | (lhs << (32 - n));
        if (raw_count != 0) {
            ARMFLAGS.C = n == 0 ? ((lhs >> 31) & 1) : ((lhs >> (n - 1)) & 1);
        }
    }
    ARMFLAGS.N = (r >> 31) & 1;
    ARMFLAGS.Z = r == 0;
    // GPR operands are unchanged.
}
