// sbc.d
// @meta category: base_integer
// @meta synopsis: sbc.d rd, rj, rk
// @meta desc: Subtract 64-bit `rk` and x86 CF from `rj`. Store the result in `rd`. Flags unchanged.
//
// @reg read: GPR[rj], GPR[rk], EFLAGS[CF]
// @reg write: GPR[rd]
// @reg preserve: EFLAGS (all), SCRATCH, x87, FPR
//
// @flag: none
// @cond: none
//
{
    uint64_t lhs = (uint64_t)GPR[rj];
    uint64_t rhs = (uint64_t)GPR[rk];
    uint8_t cf = EFLAGS.CF;
    uint64_t r = lhs - rhs - cf;
    GPR[rd] = r;
    // All flags unchanged (reads CF but does not write flags).
}
