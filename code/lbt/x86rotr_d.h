// x86rotr.d
// @meta category: x86_shift_rotate
// @meta synopsis: x86rotr.d rj, rk
// @meta desc: Compute EFLAGS (CF, OF) from 64-bit ROR of rj by count. PF/AF/ZF/SF preserved. OF defined only at
// count=1. CF=0 at count=0.
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
    uint64_t v = (uint64_t)GPR[rj];
    unsigned c = (unsigned)(GPR[rk] & 0x3f);
    unsigned n = c % 64;
    uint8_t carry_out = n == 0 ? 0 : ((v >> (n - 1)) & 1);
    uint64_t r = n == 0 ? v : (uint64_t)((v >> n) | (v << (64 - n)));
    EFLAGS.CF = carry_out;
    // PF, AF, ZF, SF preserved from input
    if (c == 1) {
        EFLAGS.OF = (((r >> (64 - 1)) ^ (r >> (64 - 2))) & 1) != 0;
    }
    // else OF preserved from input
    // GPR operands are unchanged.
}
