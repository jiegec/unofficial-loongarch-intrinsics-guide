// setarmj
// @meta category: arm
// @meta synopsis: setarmj rd, cond
// @meta desc: rd = 1 if ARM cond holds, else 0. Flags unchanged.
//
// @reg read: ARMFLAGS[N,Z,C,V]
// @reg write: GPR[rd]
// @reg preserve: ARMFLAGS, all other GPR, SCRATCH, x87, FPR
//
// @flag N: PRESERVE         unchanged
// @flag Z: PRESERVE         unchanged
// @flag C: PRESERVE         unchanged
// @flag V: PRESERVE         unchanged
// @cond: none
//
{
    bool res = arm_cond_holds(ARMFLAGS, cond);
    GPR[rd] = res ? 1 : 0;
    // ARMFLAGS unchanged.
}
