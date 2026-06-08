// x86sbc.b
// @meta category: x86_arithmetic
// @meta synopsis: x86sbc.b rj, rk
// @meta desc: Compute EFLAGS from 8-bit SUB of rj, rk, and CF (borrow).
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
    uint8_t lhs = (uint8_t)GPR[rj];
    uint8_t rhs = (uint8_t)GPR[rk];
    uint8_t carry_in = EFLAGS.CF;
    uint64_t subtrahend = (uint64_t)rhs + carry_in;
    uint8_t result = (uint8_t)(lhs - subtrahend);
    EFLAGS.CF = lhs < subtrahend;
    EFLAGS.AF = (lhs & 0xf) < (rhs & 0xf);
    EFLAGS.OF = ((lhs ^ rhs) & (lhs ^ result) & 0x80) != 0;
    EFLAGS.PF = parity_even((uint8_t)result);
    EFLAGS.ZF = result == 0;
    EFLAGS.SF = (int8_t)result < 0;
    // GPR[rj] and GPR[rk] are unchanged.
}
