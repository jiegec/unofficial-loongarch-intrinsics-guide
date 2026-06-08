// x86rcr.w
// @meta category: x86_shift_rotate
// @meta synopsis: x86rcr.w rj, rk
// @meta desc: Compute EFLAGS (CF, OF) from 32-bit RCR of rj by count through input CF. PF/AF/ZF/SF preserved. OF
// defined only at count=1.
//
// @reg read: GPR[rj], GPR[rk], EFLAGS[CF]
// @reg write: EFLAGS[CF,OF(cond)]
//
// @flag CF: READ_COMPUTE     reads input CF, writes carry-out
// @flag PF: PRESERVE         tested preserved
// @flag AF: PRESERVE         tested preserved
// @flag ZF: PRESERVE         tested preserved
// @flag SF: PRESERVE         tested preserved
// @flag OF: CONDITIONAL       [when count == 1]
// @cond: effective count == 0 => all flags PRESERVE
//
{
    uint32_t v = (uint32_t)GPR[rj];
    unsigned c = (unsigned)(GPR[rk] & 0x1f);
    unsigned n = c;
    if (n == 0)
        return; // all flags preserved
    // RCR: CF participates in rotation ring
    // PF, AF, ZF, SF preserved
    unsigned carry_in = EFLAGS.CF;
    unsigned carry_out = ((v >> ((n - 1) % (32 + 1))) & 1);
    EFLAGS.CF = carry_out;
    if (c == 1) {
        uint32_t r = (uint32_t)((v >> 1) | (carry_in << 31));
        EFLAGS.OF = (((r >> 31) ^ (r >> 30)) & 1) != 0;
    }
    // OF only when n == 1, else preserved
    // GPR operands are unchanged.
}
