// x86adc.b
// @meta category: x86_arithmetic
// @meta synopsis: x86adc.b rj, rk
// @meta desc: Compute EFLAGS from 8-bit ADD of rj, rk, and CF.
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
    uint8_t lhs = (uint8_t)GPR[rj];
    uint8_t rhs = (uint8_t)GPR[rk];
    uint8_t carry_in = EFLAGS.CF;
    uint8_t result = lhs + rhs + carry_in;
    EFLAGS.CF = (uint64_t)lhs + rhs + carry_in > UINT8_MAX;
    EFLAGS.AF = ((lhs ^ rhs ^ result) & 0x10) != 0;
    EFLAGS.OF = ((~(lhs ^ rhs)) & (lhs ^ result) & 0x80) != 0;
    EFLAGS.PF = parity_even((uint8_t)result);
    EFLAGS.ZF = result == 0;
    EFLAGS.SF = (int8_t)result < 0;
    // GPR[rj] and GPR[rk] are unchanged.
}
