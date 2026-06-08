// x86mfflag
// @meta category: overview
// @meta synopsis: x86mfflag rd, mask
// @meta desc: rd = EFLAGS bits selected by mask, zero-extended. Flags unchanged.
//
// @reg read: EFLAGS[mask-selected]
// @reg write: GPR[rd]
// @reg preserve: EFLAGS, all other GPR, SCRATCH, x87, FPR
//
// @flag CF: PRESERVE         read
// @flag PF: PRESERVE         read
// @flag AF: PRESERVE         read
// @flag ZF: PRESERVE         read
// @flag SF: PRESERVE         read
// @flag OF: PRESERVE         read
// @cond: none
//
{
    GPR[rd] = 0;
    if (mask & (1 << 0))
        GPR[rd] |= (uint64_t)EFLAGS.CF << 0;
    if (mask & (1 << 1))
        GPR[rd] |= (uint64_t)EFLAGS.PF << 2;
    if (mask & (1 << 2))
        GPR[rd] |= (uint64_t)EFLAGS.AF << 4;
    if (mask & (1 << 3))
        GPR[rd] |= (uint64_t)EFLAGS.ZF << 6;
    if (mask & (1 << 4))
        GPR[rd] |= (uint64_t)EFLAGS.SF << 7;
    if (mask & (1 << 5))
        GPR[rd] |= (uint64_t)EFLAGS.OF << 11;
    // EFLAGS unchanged.
}
