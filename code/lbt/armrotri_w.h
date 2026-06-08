// armrotri.w
// @meta category: arm
// @meta synopsis: armrotri.w rj, imm, cond
// @meta desc: Compute ARM flags from 32-bit ROR of rj. Updates N,Z,C if cond holds. V preserved.
//
// @reg read: GPR[rj], ARMFLAGS (condition)
// @reg write: ARMFLAGS[N,Z,C]
//
// @flag N: COMPUTE          sign
// @flag Z: COMPUTE          zero
// @flag C: COMPUTE          carry-out
// @flag V: PRESERVE         preserved
// @cond: cond false => all writes suppressed and state preserved
//
{
    if (!arm_cond_holds(ARMFLAGS, cond))
        return;
    uint32_t v = (uint32_t)GPR[rj];
    unsigned n = imm % 32;
    uint32_t r = n == 0 ? v : ((v >> n) | (v << (32 - n)));
    if (imm != 0) {
        ARMFLAGS.C = n == 0 ? ((v >> 31) & 1) : ((v >> (n - 1)) & 1);
    }
    ARMFLAGS.N = (r >> 31) & 1;
    ARMFLAGS.Z = r == 0;
    // GPR unchanged.
}
