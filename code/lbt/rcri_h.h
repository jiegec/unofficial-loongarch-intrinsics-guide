// rcri.h
// @meta category: base_integer
// @meta synopsis: rcri.h rd, rj, imm
// @meta desc: Rotate right 16-bit `rj` by `imm` through x86 CF. Store the result in `rd`. Flags unchanged.
//
// @reg read: GPR[rj], EFLAGS[CF]
// @reg write: GPR[rd]
// @reg preserve: EFLAGS (all), SCRATCH, x87, FPR
//
// @flag: none
// @cond: none
//
{
    uint16_t v = (uint16_t)GPR[rj];
    unsigned c = (unsigned)imm % (16 + 1);
    // Rotate [CF, v] right by c, write low 16 bits to rd
    uint64_t ring_hi = EFLAGS.CF;
    uint64_t ring_lo = v;
    uint64_t r;
    if (c == 0) {
        r = v;
    } else {
        uint64_t tmp_hi = ring_hi, tmp_lo = ring_lo;
        for (unsigned i = 0; i < c; i++) {
            uint64_t new_lo = (tmp_lo >> 1) | (tmp_hi << (16 - 1));
            uint64_t new_hi = tmp_lo & 1;
            tmp_lo = new_lo;
            tmp_hi = new_hi;
        }
        r = tmp_lo & ((1ULL << 16) - 1);
    }
    GPR[rd] = sext(r, 16);
    // CF input participates in ring; all flags unchanged after.
}
