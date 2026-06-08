// x86sll.w
// @meta category: x86_shift_rotate
// @meta synopsis: x86sll.w rj, rk
// @meta desc: Compute EFLAGS from 32-bit SHL (logical left) of rj by count. OF defined only at count=1.
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
    uint32_t v = (uint32_t)GPR[rj];
    unsigned c = (unsigned)(GPR[rk] & 0x1f);
    if (c == 0)
        return; // all flags preserved

    // c is in [1,31] after mask & early return; always a valid shift.
    uint8_t carry_out = (v >> (32 - c)) & 1;
    uint32_t r = (uint32_t)(v << c);
    EFLAGS.CF = carry_out;
    EFLAGS.PF = parity_even((uint8_t)r);
    // AF preserved from input
    EFLAGS.ZF = r == 0;
    EFLAGS.SF = (int32_t)r < 0;
    if (c == 1) {
        EFLAGS.OF = ((v ^ r) & 0x80000000) != 0;
    }
    // else OF preserved from input
    // GPR operands are unchanged.
}
