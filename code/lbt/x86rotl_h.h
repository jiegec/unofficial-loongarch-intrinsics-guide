// x86rotl.h
// @meta category: x86_shift_rotate
// @meta synopsis: x86rotl.h rj, rk
// @meta desc: Compute EFLAGS (CF, OF) from 16-bit ROL of rj by count. PF/AF/ZF/SF preserved. OF defined only at
// count=1. CF=MSB at count=0.
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
    uint16_t v = (uint16_t)GPR[rj];
    unsigned c = (unsigned)(GPR[rk] & 0x1f);
    unsigned n = c % 16;
    uint8_t carry_out = n == 0 ? ((v >> (16 - 1)) & 1) : ((v >> (16 - n)) & 1);
    uint16_t r = n == 0 ? v : (uint16_t)((v << n) | (v >> (16 - n)));
    EFLAGS.CF = carry_out;
    // PF, AF, ZF, SF preserved from input
    if (c == 1) {
        EFLAGS.OF = ((v ^ r) & 0x8000) != 0;
    }
    // else OF preserved from input
    // GPR operands are unchanged.
}
