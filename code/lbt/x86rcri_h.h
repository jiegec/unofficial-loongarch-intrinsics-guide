// x86rcri.h
// @meta category: x86_shift_rotate
// @meta synopsis: x86rcri.h rj, imm
// @meta desc: Compute EFLAGS (CF, OF) from 16-bit RCR of rj by count through input CF. PF/AF/ZF/SF preserved. OF
// defined only at count=1.
//
// @reg read: GPR[rj], EFLAGS[CF]
// @reg write: EFLAGS[CF,OF(cond)]
//
// @flag CF: READ_COMPUTE     carry-out
// @flag PF: PRESERVE         tested preserved
// @flag AF: PRESERVE         tested preserved
// @flag ZF: PRESERVE         tested preserved
// @flag SF: PRESERVE         tested preserved
// @flag OF: CONDITIONAL       [when count == 1]
// @cond: effective count == 0 => all flags PRESERVE
//
{
    uint16_t v = (uint16_t)GPR[rj];
    unsigned c = (unsigned)imm;
    unsigned n = c % 17;
    if (n == 0)
        return; // all flags preserved
    // RCR: CF participates in rotation ring
    // PF, AF, ZF, SF preserved
    unsigned carry_in = EFLAGS.CF;
    unsigned carry_out = ((v >> ((n - 1) % (16 + 1))) & 1);
    EFLAGS.CF = carry_out;
    if (c == 1) {
        uint16_t r = (uint16_t)((v >> 1) | (carry_in << 15));
        EFLAGS.OF = (((r >> 15) ^ (r >> 14)) & 1) != 0;
    }
    // OF only when n == 1, else preserved
    // GPR operands are unchanged.
}
