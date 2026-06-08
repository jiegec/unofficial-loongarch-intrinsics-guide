// rotri.b
// @meta category: base_integer
// @meta synopsis: rotri.b rd, rj, imm
// @meta desc: Rotate right 8-bit `rj` by `imm`. Store the result in `rd`. Flags unchanged.
//
// @reg read: GPR[rj]
// @reg write: GPR[rd]
// @reg preserve: EFLAGS (all), SCRATCH, x87, FPR
//
// @flag: none
// @cond: none
//
{
    uint8_t v = (uint8_t)GPR[rj];
    unsigned c = imm % 8;
    uint8_t r = c == 0 ? v : (uint8_t)((v >> c) | (v << (8 - c)));
    GPR[rd] = sext(r, 8);
    // All flags unchanged.
}
