// rotr.w
// @meta category: base_integer
// @meta synopsis: rotr.w rd, rj, rk
// @meta desc: Rotate right 32-bit `rj` by `rk`. Store the result in `rd`. Flags unchanged.
//
// @reg read: GPR[rj], GPR[rk]
// @reg write: GPR[rd]
// @reg preserve: EFLAGS (all), SCRATCH, x87, FPR
//
// @flag: none
// @cond: none
//
{
    uint32_t v = (uint32_t)GPR[rj];
    unsigned c = (unsigned)GPR[rk] % 32;
    uint32_t r = c == 0 ? v : (uint32_t)((v >> c) | (v << (32 - c)));
    GPR[rd] = sext(r, 32);
    // All flags unchanged.
}
