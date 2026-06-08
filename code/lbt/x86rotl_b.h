// x86rotl.b
// @meta category: x86_shift_rotate
// @meta synopsis: x86rotl.b rj, rk
// @meta desc: Compute EFLAGS (CF, OF) from 8-bit ROL of rj by count. PF/AF/ZF/SF preserved. OF defined only at count=1.
// CF=MSB at count=0.
//
// @reg read: GPR[rj], GPR[rk]
// @reg write: EFLAGS[CF,OF(cond)]
//
// @flag CF: COMPUTE          rotate carry-out; written even when count==0
// @flag PF: PRESERVE         tested preserved
// @flag AF: PRESERVE         tested preserved
// @flag ZF: PRESERVE         tested preserved
// @flag SF: PRESERVE         tested preserved
// @flag OF: CONDITIONAL       [when raw_count == 1]
// @cond: none
//
{
    uint8_t v = (uint8_t)GPR[rj];
    unsigned c = (unsigned)(GPR[rk] & 0x1f);
    unsigned n = c % 8;
    uint8_t carry_out = n == 0 ? ((v >> (8 - 1)) & 1) : ((v >> (8 - n)) & 1);
    uint8_t r = n == 0 ? v : (uint8_t)((v << n) | (v >> (8 - n)));
    EFLAGS.CF = carry_out;
    // PF, AF, ZF, SF preserved from input
    if (c == 1) {
        EFLAGS.OF = ((v ^ r) & 0x80) != 0;
    }
    // else OF preserved from input
    // GPR operands are unchanged.
}
