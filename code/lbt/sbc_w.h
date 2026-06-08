// sbc.w
// @meta category: base_integer
// @meta synopsis: sbc.w rd, rj, rk
// @meta desc: Subtract 32-bit `rk` and x86 CF from `rj`. Store the result in `rd`. Flags unchanged.
//
// @reg read: GPR[rj], GPR[rk], EFLAGS[CF]
// @reg write: GPR[rd]
// @reg preserve: EFLAGS (all), SCRATCH, x87, FPR
//
// @flag: none
// @cond: none
//
{
    uint32_t lhs = (uint32_t)GPR[rj];
    uint32_t rhs = (uint32_t)GPR[rk];
    uint8_t cf = EFLAGS.CF;
    uint32_t r = lhs - rhs - cf;
    GPR[rd] = sext(r, 32);
    // All flags unchanged (reads CF but does not write flags).
}
