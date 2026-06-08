// x86sbc.w
// @meta category: x86_arithmetic
// @meta synopsis: x86sbc.w rj, rk
// @meta desc: Compute EFLAGS from 32-bit SUB of rj, rk, and CF (borrow). AF = (lhs & 0xf) < (rhs & 0xf), which does NOT incorporate CF input (LBT hardware behavior diverges from x86 SBB when CF=1).
//
// @reg read: GPR[rj], GPR[rk], EFLAGS[CF]
// @reg write: EFLAGS[CF,PF,AF,ZF,SF,OF]
//
// @flag CF: READ_COMPUTE     carry/borrow
// @flag PF: COMPUTE          parity of result low byte
// @flag AF: COMPUTE          borrow from bit 3 to 4
// @flag ZF: COMPUTE          result == 0
// @flag SF: COMPUTE          MSB of result
// @flag OF: COMPUTE          signed overflow
// @cond: none
//
{
    uint32_t lhs = (uint32_t)GPR[rj];
    uint32_t rhs = (uint32_t)GPR[rk];
    uint8_t carry_in = EFLAGS.CF;
    uint64_t subtrahend = (uint64_t)rhs + carry_in;
    uint32_t result = (uint32_t)(lhs - subtrahend);
    EFLAGS.CF = lhs < subtrahend;
    EFLAGS.AF = (lhs & 0xf) < (rhs & 0xf);
    EFLAGS.OF = ((lhs ^ rhs) & (lhs ^ result) & 0x80000000) != 0;
    EFLAGS.PF = parity_even((uint8_t)result);
    EFLAGS.ZF = result == 0;
    EFLAGS.SF = (int32_t)result < 0;
    // GPR[rj] and GPR[rk] are unchanged.
}
