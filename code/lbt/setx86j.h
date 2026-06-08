// setx86j
// @meta category: x86_control
// @meta synopsis: setx86j rd, cond
// @meta desc: rd = 1 if x86 cond holds, else 0. Flags unchanged.
//
// @reg read: EFLAGS[CF,PF,ZF,SF,OF]
// @reg write: GPR[rd]
// @reg preserve: EFLAGS, all other GPR, SCRATCH, x87, FPR
//
// @flag CF: PRESERVE         condition input
// @flag PF: PRESERVE         used by PE/PO conditions
// @flag AF: PRESERVE         unchanged
// @flag ZF: PRESERVE         read for condition
// @flag SF: PRESERVE         condition input
// @flag OF: PRESERVE         condition input
// @cond: none
//
{
    bool res = x86_cond_holds(EFLAGS, cond);
    GPR[rd] = res ? 1 : 0;
    // EFLAGS unchanged.
}
