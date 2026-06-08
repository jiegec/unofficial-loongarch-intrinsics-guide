// sbc.h
// @meta category: base_integer
// @meta synopsis: sbc.h rd, rj, rk
// @meta desc: Subtract 16-bit `rk` and x86 CF from `rj`. Store the result in `rd`. Flags unchanged.
//
// @reg read: GPR[rj], GPR[rk], EFLAGS[CF]
// @reg write: GPR[rd]
// @reg preserve: EFLAGS (all), SCRATCH, x87, FPR
//
// @flag: none
// @cond: none
//
{
    uint16_t lhs = (uint16_t)GPR[rj];
    uint16_t rhs = (uint16_t)GPR[rk];
    uint8_t cf = EFLAGS.CF;
    uint16_t r = lhs - rhs - cf;
    GPR[rd] = sext(r, 16);
    // All flags unchanged (reads CF but does not write flags).
}
