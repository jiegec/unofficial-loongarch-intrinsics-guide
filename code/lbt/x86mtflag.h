// x86mtflag
// @meta category: overview
// @meta synopsis: x86mtflag rj, mask
// @meta desc: Set EFLAGS bits selected by mask from rj. Other flags unchanged.
//
// @reg read: GPR[rj]
// @reg write: EFLAGS[mask-selected]
//
// @flag CF: COMPUTE          written from GPR
// @flag PF: COMPUTE          written from GPR
// @flag AF: COMPUTE          written from GPR
// @flag ZF: COMPUTE          written from GPR
// @flag SF: COMPUTE          written from GPR
// @flag OF: COMPUTE          written from GPR
// @cond: none
//
{
    uint64_t v = GPR[rj];
    if (mask & (1 << 0))
        EFLAGS.CF = (v >> 0) & 1;
    if (mask & (1 << 1))
        EFLAGS.PF = (v >> 2) & 1;
    if (mask & (1 << 2))
        EFLAGS.AF = (v >> 4) & 1;
    if (mask & (1 << 3))
        EFLAGS.ZF = (v >> 6) & 1;
    if (mask & (1 << 4))
        EFLAGS.SF = (v >> 7) & 1;
    if (mask & (1 << 5))
        EFLAGS.OF = (v >> 11) & 1;
    // GPR unchanged.
}
