// x86sub.du
// @meta category: x86_arithmetic
// @meta synopsis: x86sub.du rj, rk
// @meta desc: Compute EFLAGS from 64-bit unsigned SUB of rj and rk. OF=0 (unsigned).
//
// @reg read: GPR[rj], GPR[rk]
// @reg write: EFLAGS[CF,PF,AF,ZF,SF,OF]
//
// @flag CF: COMPUTE          carry/borrow
// @flag PF: COMPUTE          parity of result low byte
// @flag AF: COMPUTE          carry/borrow from bit 3 to 4
// @flag ZF: COMPUTE          result == 0
// @flag SF: COMPUTE          MSB of result
// @flag OF: FORCE_0          unsigned — no signed overflow
// @cond: none
//
{
    uint64_t lhs = (uint64_t)GPR[rj];
    uint64_t rhs = (uint64_t)GPR[rk];
    uint64_t subtrahend = (uint64_t)rhs;
    uint64_t result = (uint64_t)(lhs - subtrahend);
    EFLAGS.CF = lhs < subtrahend;
    EFLAGS.AF = ((lhs ^ rhs ^ result) & 0x10) != 0;
    EFLAGS.OF = 0;
    EFLAGS.PF = parity_even((uint8_t)result);
    EFLAGS.ZF = result == 0;
    EFLAGS.SF = (int64_t)result < 0;
    // GPR[rj] and GPR[rk] are unchanged.
}
