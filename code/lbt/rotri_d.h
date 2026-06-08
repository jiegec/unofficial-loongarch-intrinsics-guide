// rotri.d
// @meta category: base_integer
// @meta synopsis: rotri.d rd, rj, imm
// @meta desc: Rotate right 64-bit `rj` by `imm`. Store the result in `rd`. Flags unchanged.
//
// @reg read: GPR[rj]
// @reg write: GPR[rd]
// @reg preserve: EFLAGS (all), SCRATCH, x87, FPR
//
// @flag: none
// @cond: none
//
{
    uint64_t v = (uint64_t)GPR[rj];
    unsigned c = imm % 64;
    uint64_t r = c == 0 ? v : (uint64_t)((v >> c) | (v << (64 - c)));
    GPR[rd] = r;
    // All flags unchanged.
}
