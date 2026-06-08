// adc.b
// @meta category: base_integer
// @meta synopsis: adc.b rd, rj, rk
// @meta desc: Add 8-bit values in `rj` and `rk` with x86 CF. Store the result in `rd`. Flags unchanged.
//
// @reg read: GPR[rj], GPR[rk], EFLAGS[CF]
// @reg write: GPR[rd]
// @reg preserve: EFLAGS (all), SCRATCH, x87, FPR
//
// @flag: none
// @cond: none
//
{
    uint8_t lhs = (uint8_t)GPR[rj];
    uint8_t rhs = (uint8_t)GPR[rk];
    uint8_t cf = EFLAGS.CF;
    uint8_t r = lhs + rhs + cf;
    GPR[rd] = sext(r, 8);
    // All flags unchanged (reads CF but does not write flags).
}
