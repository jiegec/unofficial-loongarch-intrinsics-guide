// armrrx.w
// @meta category: arm
// @meta synopsis: armrrx.w rj, cond
// @meta desc: Compute ARM flags from 32-bit RRX of rj through C. Updates N,Z,C if cond holds. V preserved.
//
// @reg read: GPR[rj], ARMFLAGS (condition), ARMFLAGS[C]
// @reg write: ARMFLAGS[N,Z,C]
// @reg preserve: GPR[rj], all other GPR, SCRATCH, x87, FPR
//
// @flag N: COMPUTE          sign
// @flag Z: COMPUTE          zero
// @flag C: READ_COMPUTE     carry-out
// @flag V: PRESERVE         preserved
// @cond: cond false => all writes suppressed and state preserved
//
{
    if (!arm_cond_holds(ARMFLAGS, cond))
        return;
    uint32_t v = (uint32_t)GPR[rj];
    uint32_t r = (v >> 1) | (ARMFLAGS.C ? 0x80000000 : 0);
    ARMFLAGS.C = v & 1;
    ARMFLAGS.N = (r >> 31) & 1;
    ARMFLAGS.Z = r == 0;
    // V unchanged
    // GPR operands unchanged.
}
