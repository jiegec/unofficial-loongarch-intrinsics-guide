// armmove
// @meta category: arm
// @meta synopsis: armmove rd, rj, cond
// @meta desc: rd = rj if ARM cond holds, else rd unchanged. Flags unchanged.
//
// @reg read: GPR[rj], ARMFLAGS (condition)
// @reg write: GPR[rd]
// @reg preserve: ARMFLAGS, all other GPR, SCRATCH, x87
//
// @flag N: PRESERVE         unchanged
// @flag Z: PRESERVE         unchanged
// @flag C: PRESERVE         unchanged
// @flag V: PRESERVE         unchanged
// @cond: cond false => all writes suppressed and state preserved
//
{
    if (!arm_cond_holds(ARMFLAGS, cond))
        return;
    GPR[rd] = GPR[rj];
    // ARMFLAGS unchanged.
}
