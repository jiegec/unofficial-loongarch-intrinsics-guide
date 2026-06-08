// x86sra.h
// @meta category: x86_shift_rotate
// @meta synopsis: x86sra.h rj, rk
// @meta desc: Compute EFLAGS from 16-bit SAR (arithmetic right) of rj by count. OF=0 at count=1.
//
// @reg read: GPR[rj], GPR[rk]
// @reg write: EFLAGS[CF,PF,ZF,SF,OF(cond)]
//
// @flag CF: COMPUTE          last bit shifted out; 0 when count > width
// @flag PF: COMPUTE          parity of result low byte
// @flag AF: PRESERVE         tested preserved
// @flag ZF: COMPUTE          result == 0
// @flag SF: COMPUTE          MSB of result
// @flag OF: CONDITIONAL      MSB changed for left; original MSB for right/sra [when raw_count == 1]
// @cond: count == 0 => all flags PRESERVE (unchanged)
//
{
    uint16_t v = (uint16_t)GPR[rj];
    unsigned c = (unsigned)(GPR[rk] & 0x1f);
    if (c == 0)
        return; // all flags preserved

    uint8_t carry_out = c > 16 ? ((v >> (16 - 1)) & 1) : ((v >> (c - 1)) & 1);
    uint16_t r = c >= 16 ? ((int16_t)v < 0 ? (uint16_t)-1 : 0) : (uint16_t)((int16_t)v >> c);
    EFLAGS.CF = carry_out;
    EFLAGS.PF = parity_even((uint8_t)r);
    // AF preserved from input
    EFLAGS.ZF = r == 0;
    EFLAGS.SF = (int16_t)r < 0;
    if (c == 1) {
        EFLAGS.OF = 0;
    }
    // else OF preserved from input
    // GPR operands are unchanged.
}
