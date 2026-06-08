// x86rcl.d
// @meta category: x86_shift_rotate
// @meta synopsis: x86rcl.d rj, rk
// @meta desc: Compute EFLAGS (CF, OF) from 64-bit RCL of rj by count through input CF. PF/AF/ZF/SF preserved. OF
// defined only at count=1.
//
// @reg read: GPR[rj], GPR[rk], EFLAGS[CF]
// @reg write: EFLAGS[CF,OF(cond)]
//
// @flag CF: READ_COMPUTE     carry-out; does NOT read input CF when n != 0
// @flag PF: PRESERVE         tested preserved
// @flag AF: PRESERVE         tested preserved
// @flag ZF: PRESERVE         tested preserved
// @flag SF: PRESERVE         tested preserved
// @flag OF: CONDITIONAL      MSB changed [when count == 1]
// @cond: effective count == 0 => all flags PRESERVE
//
{
    uint64_t v = (uint64_t)GPR[rj];
    unsigned c = (unsigned)(GPR[rk] & 0x3f);
    unsigned n = c;
    if (n == 0)
        return; // all flags preserved (including CF)
    // RCL: shift left through CF, CF fills LSB
    unsigned carry_out = ((v >> (64 - n)) & 1);
    // PF, AF, ZF, SF preserved
    EFLAGS.CF = carry_out;
    if (c == 1) {
        uint64_t r = (uint64_t)(v << 1);
        EFLAGS.OF = ((r & 0x8000000000000000ULL) != 0) != carry_out;
    }
    // else OF preserved from input
    // GPR operands are unchanged.
}
