// armmfflag
// @meta category: overview
// @meta synopsis: armmfflag rd, mask
// @meta desc: rd = ARM NZCV bits selected by mask, sign-extended. Flags unchanged.
//
// @reg read: ARMFLAGS[mask-selected]
// @reg write: GPR[rd]
// @reg preserve: ARMFLAGS, all other GPR, SCRATCH, x87, FPR
//
// @flag N: PRESERVE         read
// @flag Z: PRESERVE         read
// @flag C: PRESERVE         read
// @flag V: PRESERVE         read
// @cond: none
//
{
    GPR[rd] = 0;
    if (mask & (1 << 4))
        GPR[rd] |= (uint64_t)ARMFLAGS.N << 31;
    if (mask & (1 << 3))
        GPR[rd] |= (uint64_t)ARMFLAGS.Z << 30;
    if (mask & (1 << 0))
        GPR[rd] |= (uint64_t)ARMFLAGS.C << 29;
    if (mask & (1 << 5))
        GPR[rd] |= (uint64_t)ARMFLAGS.V << 28;
    GPR[rd] = sext(GPR[rd], 32);
    // ARMFLAGS unchanged.
}
