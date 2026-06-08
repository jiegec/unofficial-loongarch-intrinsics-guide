// x86mul.du
// @meta category: x86_arithmetic
// @meta synopsis: x86mul.du rj, rk
// @meta desc: Compute EFLAGS from unsigned 64-bit MUL of rj and rk. CF=OF=overflow; SF/ZF/AF/PF=0.
//
// @reg read: GPR[rj], GPR[rk]
// @reg write: EFLAGS[CF,PF,AF,ZF,SF,OF]
//
// @flag CF: COMPUTE          1 when full product doesn't fit width
// @flag PF: FORCE_0          tested cleared
// @flag AF: FORCE_0          tested cleared
// @flag ZF: FORCE_0          tested cleared
// @flag SF: FORCE_0          tested cleared
// @flag OF: COMPUTE          same as CF
// @cond: none
//
{
    uint64_t lhs = (uint64_t)GPR[rj];
    uint64_t rhs = (uint64_t)GPR[rk];
    unsigned __int128 product = (unsigned __int128)(uint64_t)lhs * (unsigned __int128)(uint64_t)rhs;
    bool overflow = (product >> 64) != 0;
    EFLAGS.CF = overflow;
    EFLAGS.OF = overflow;
    EFLAGS.SF = 0;
    EFLAGS.ZF = 0;
    EFLAGS.AF = 0;
    EFLAGS.PF = 0;
    // GPR operands are unchanged.
}
