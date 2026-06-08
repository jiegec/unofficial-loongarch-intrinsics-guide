// movgr2scr
// @meta category: overview
// @meta synopsis: movgr2scr crd, rj
// @meta desc: SCRATCH[crd] = GPR[rj]. Flags unchanged.
//
// @reg read: GPR[rj]
// @reg write: SCRATCH[crd]
// @reg preserve: GPR[rj], all other SCRATCH, EFLAGS, ARMFLAGS, x87, FPR
//
// @flag: none
// @cond: none
//
{
    SCRATCH[crd] = GPR[rj];
    // No flags affected.
}
