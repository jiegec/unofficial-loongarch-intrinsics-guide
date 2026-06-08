// x86slli.h
// @meta category: x86_shift_rotate
// @meta synopsis: x86slli.h rj, imm
// @meta desc: Compute EFLAGS from 16-bit SHL (logical left) of rj by count. OF defined only at count=1.
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
    uint16_t v = (uint16_t)GPR[rj];
    unsigned c = (unsigned)imm;
    if (c == 0)
        return; // all flags preserved

    uint8_t carry_out = c > 16 ? 0 : ((v >> (16 - c)) & 1);
    uint16_t r = c >= 16 ? 0 : (uint16_t)(v << c);
    EFLAGS.CF = carry_out;
    EFLAGS.PF = parity_even((uint8_t)r);
    // AF preserved from input
    EFLAGS.ZF = r == 0;
    EFLAGS.SF = (int16_t)r < 0;
    if (c == 1) {
        EFLAGS.OF = ((v ^ r) & 0x8000) != 0;
    }
    // else OF preserved from input
    // GPR operands are unchanged.
}
