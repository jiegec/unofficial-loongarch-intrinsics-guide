// armmtflag
// @meta category: overview
// @meta synopsis: armmtflag rj, mask
// @meta desc: Set ARM NZCV bits selected by mask from rj. Other flags unchanged.
//
// @reg read: GPR[rj]
// @reg write: ARMFLAGS[mask-selected]
//
// @flag N: COMPUTE          written from GPR
// @flag Z: COMPUTE          written from GPR
// @flag C: COMPUTE          written from GPR
// @flag V: COMPUTE          written from GPR
// @cond: none
//
{
    uint64_t v = GPR[rj];
    if (mask & (1 << 4))
        ARMFLAGS.N = (v >> 31) & 1;
    if (mask & (1 << 3))
        ARMFLAGS.Z = (v >> 30) & 1;
    if (mask & (1 << 0))
        ARMFLAGS.C = (v >> 29) & 1;
    if (mask & (1 << 5))
        ARMFLAGS.V = (v >> 28) & 1;
    // GPR unchanged.
}
