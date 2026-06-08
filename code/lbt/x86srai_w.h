// x86srai.w
// @meta category: x86_shift_rotate
// @meta synopsis: x86srai.w rj, imm
// @meta desc: Compute EFLAGS from 32-bit SAR (arithmetic right) of rj by count. OF=0 at count=1.
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
    uint32_t v = (uint32_t)GPR[rj];
    unsigned c = (unsigned)imm;
    if (c == 0)
        return; // all flags preserved

    uint8_t carry_out = c > 32 ? ((v >> (32 - 1)) & 1) : ((v >> (c - 1)) & 1);
    uint32_t r = c >= 32 ? ((int32_t)v < 0 ? (uint32_t)-1 : 0) : (uint32_t)((int32_t)v >> c);
    EFLAGS.CF = carry_out;
    EFLAGS.PF = parity_even((uint8_t)r);
    // AF preserved from input
    EFLAGS.ZF = r == 0;
    EFLAGS.SF = (int32_t)r < 0;
    if (c == 1) {
        EFLAGS.OF = 0;
    }
    // else OF preserved from input
    // GPR operands are unchanged.
}
