// rotr.b
// @meta category: base_integer
// @meta synopsis: rotr.b rd, rj, rk
// @meta desc: Rotate right 8-bit `rj` by `rk`. Store the result in `rd`. Flags unchanged.
//
// @reg read: GPR[rj], GPR[rk]
// @reg write: GPR[rd]
// @reg preserve: EFLAGS (all), SCRATCH, x87, FPR
//
// @flag: none
// @cond: none
//
{
    uint8_t v = (uint8_t)GPR[rj];
    unsigned c = (unsigned)GPR[rk] % 8;
    uint8_t r = c == 0 ? v : (uint8_t)((v >> c) | (v << (8 - c)));
    GPR[rd] = sext(r, 8);
    // All flags unchanged.
}
