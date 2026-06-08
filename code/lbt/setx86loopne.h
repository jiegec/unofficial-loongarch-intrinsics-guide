// setx86loopne
// @meta category: x86_control
// @meta synopsis: setx86loopne rd, rj
// @meta desc: Decrement 64-bit rj; rd = 1 if rj != 0 and ZF = 0, else 0. GPR[j] gets decremented value.
//
// @reg read: GPR[rj], EFLAGS[ZF]
// @reg write: GPR[rd]
// @reg preserve: GPR[rj], EFLAGS, all other GPR, SCRATCH, x87, FPR
//
// @flag CF: PRESERVE         unchanged
// @flag PF: PRESERVE         unchanged
// @flag AF: PRESERVE         unchanged
// @flag ZF: PRESERVE         read for condition
// @flag SF: PRESERVE         unchanged
// @flag OF: PRESERVE         unchanged
// @cond: none
//
{
    uint64_t counter_after_dec = GPR[rj];
    GPR[rd] = (counter_after_dec != 0 && !EFLAGS.ZF) ? 1 : 0;
    // GPR[rj] and EFLAGS unchanged.
}
