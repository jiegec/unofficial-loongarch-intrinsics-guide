// rotri.w
// @meta category: base_integer
// @meta synopsis: rotri.w rd, rj, imm
// @meta desc: Rotate right 32-bit `rj` by `imm`. Store the result in `rd`. Flags unchanged.
//
// @reg read: GPR[rj]
// @reg write: GPR[rd]
// @reg preserve: EFLAGS (all), SCRATCH, x87, FPR
//
// @flag: none
// @cond: none
//
{
    uint32_t v = (uint32_t)GPR[rj];
    unsigned c = imm % 32;
    uint32_t r = c == 0 ? v : (uint32_t)((v >> c) | (v << (32 - c)));
    GPR[rd] = sext(r, 32);
    // All flags unchanged.
}
