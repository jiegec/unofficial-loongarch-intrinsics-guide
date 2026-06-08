// armmov.w
// @meta category: arm
// @meta synopsis: armmov.w rj, cond
// @meta desc: Update ARM N,Z from 32-bit rj if cond holds. C,V preserved.
//
// @reg read: GPR[rj], ARMFLAGS (condition)
// @reg write: ARMFLAGS[N,Z]
// @reg preserve: GPR[rj], all other GPR, SCRATCH, x87, FPR
//
// @flag N: COMPUTE          sign
// @flag Z: COMPUTE          zero
// @flag C: PRESERVE         preserved
// @flag V: PRESERVE         preserved
// @cond: cond false => all writes suppressed and state preserved
//
{
    if (!arm_cond_holds(ARMFLAGS, cond))
        return;
    uint32_t r = (uint32_t)GPR[rj];
    ARMFLAGS.N = (r >> 31) & 1;
    ARMFLAGS.Z = r == 0;
    // C, V unchanged
    // GPR operands unchanged.
}
