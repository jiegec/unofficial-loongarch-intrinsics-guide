// rotr.d
// @meta category: base_integer
// @meta synopsis: rotr.d rd, rj, rk
// @meta desc: Rotate right 64-bit `rj` by `rk`. Store the result in `rd`. Flags unchanged.
//
// @reg read: GPR[rj], GPR[rk]
// @reg write: GPR[rd]
// @reg preserve: EFLAGS (all), SCRATCH, x87, FPR
//
// @flag: none
// @cond: none
//
{
    uint64_t v = (uint64_t)GPR[rj];
    unsigned c = (unsigned)GPR[rk] % 64;
    uint64_t r = c == 0 ? v : (uint64_t)((v >> c) | (v << (64 - c)));
    GPR[rd] = r;
    // All flags unchanged.
}
