// x86adc.d
// @meta category: x86_arithmetic
// @meta synopsis: x86adc.d rj, rk
// @meta desc: Compute EFLAGS from 64-bit ADD of rj, rk, and CF.
//
// @reg read: GPR[rj], GPR[rk], EFLAGS[CF]
// @reg write: EFLAGS[CF,PF,AF,ZF,SF,OF]
//
// @flag CF: READ_COMPUTE     carry/borrow
// @flag PF: COMPUTE          parity of result low byte
// @flag AF: COMPUTE          carry from bit 3 to 4
// @flag ZF: COMPUTE          result == 0
// @flag SF: COMPUTE          MSB of result
// @flag OF: COMPUTE          signed overflow
// @cond: none
//
{
    uint64_t lhs = (uint64_t)GPR[rj];
    uint64_t rhs = (uint64_t)GPR[rk];
    uint8_t carry_in = EFLAGS.CF;
    uint64_t result = lhs + rhs + carry_in;
    EFLAGS.CF = lhs > UINT64_MAX - rhs - carry_in;
    EFLAGS.AF = ((lhs ^ rhs ^ result) & 0x10) != 0;
    EFLAGS.OF = ((~(lhs ^ rhs)) & (lhs ^ result) & 0x8000000000000000ULL) != 0;
    EFLAGS.PF = parity_even((uint8_t)result);
    EFLAGS.ZF = result == 0;
    EFLAGS.SF = (int64_t)result < 0;
    // GPR[rj] and GPR[rk] are unchanged.
}
