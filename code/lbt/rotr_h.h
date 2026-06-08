// rotr.h
// @meta category: base_integer
// @meta synopsis: rotr.h rd, rj, rk
// @meta desc: Rotate right 16-bit `rj` by `rk`. Store the result in `rd`. Flags unchanged.
//
// @reg read: GPR[rj], GPR[rk]
// @reg write: GPR[rd]
// @reg preserve: EFLAGS (all), SCRATCH, x87, FPR
//
// @flag: none
// @cond: none
//
{
    uint16_t v = (uint16_t)GPR[rj];
    unsigned c = (unsigned)GPR[rk] % 16;
    uint16_t r = c == 0 ? v : (uint16_t)((v >> c) | (v << (16 - c)));
    GPR[rd] = sext(r, 16);
    // All flags unchanged.
}
