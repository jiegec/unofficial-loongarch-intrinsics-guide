// movscr2gr
// @meta category: overview
// @meta synopsis: movscr2gr rd, crj
// @meta desc: GPR[rd] = SCRATCH[crj]. Flags unchanged.
//
// @reg read: SCRATCH[crj]
// @reg write: GPR[rd]
// @reg preserve: SCRATCH[crj], all other GPR, EFLAGS, ARMFLAGS, x87, FPR
//
// @flag: none
// @cond: none
//
{
    GPR[rd] = SCRATCH[crj];
    // No flags affected.
}
