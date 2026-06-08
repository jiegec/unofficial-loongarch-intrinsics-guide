// x86rcli.b
// @meta category: x86_shift_rotate
// @meta synopsis: x86rcli.b rj, imm
// @meta desc: Compute EFLAGS (CF, OF) from 8-bit RCL of rj by count through input CF. PF/AF/ZF/SF preserved. OF defined
// only at count=1.
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
    uint8_t v = (uint8_t)GPR[rj];
    unsigned c = (unsigned)imm;
    unsigned n = c % 9;
    if (n == 0)
        return; // all flags preserved (including CF)
    // RCL: shift left through CF, CF fills LSB
    unsigned carry_out = ((v >> (8 - n)) & 1);
    // PF, AF, ZF, SF preserved
    EFLAGS.CF = carry_out;
    if (c == 1) {
        uint8_t r = (uint8_t)(v << 1);
        EFLAGS.OF = ((r & 0x80) != 0) != carry_out;
    }
    // else OF preserved from input
    // GPR operands are unchanged.
}
