// x86srai.b
// @meta category: x86_shift_rotate
// @meta synopsis: x86srai.b rj, imm
// @meta desc: Compute EFLAGS from 8-bit SAR (arithmetic right) of rj by count. OF=0 at count=1.
//
// @reg read: GPR[rj]
// @reg write: EFLAGS[CF,PF,ZF,SF,OF(cond)]
//
// @flag CF: COMPUTE          last bit shifted out
// @flag PF: COMPUTE          parity of result low byte
// @flag AF: PRESERVE         tested preserved
// @flag ZF: COMPUTE          result == 0
// @flag SF: COMPUTE          MSB of result
// @flag OF: CONDITIONAL       [when raw_count == 1]
// @cond: count == 0 => all flags PRESERVE
//
{
    uint8_t v = (uint8_t)GPR[rj];
    unsigned c = (unsigned)imm;
    if (c == 0)
        return; // all flags preserved

    uint8_t carry_out = c > 8 ? ((v >> (8 - 1)) & 1) : ((v >> (c - 1)) & 1);
    uint8_t r = c >= 8 ? ((int8_t)v < 0 ? (uint8_t)-1 : 0) : (uint8_t)((int8_t)v >> c);
    EFLAGS.CF = carry_out;
    EFLAGS.PF = parity_even((uint8_t)r);
    // AF preserved from input
    EFLAGS.ZF = r == 0;
    EFLAGS.SF = (int8_t)r < 0;
    if (c == 1) {
        EFLAGS.OF = 0;
    }
    // else OF preserved from input
    // GPR operands are unchanged.
}
