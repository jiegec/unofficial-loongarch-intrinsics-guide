// x86add.wu
// @meta category: x86_arithmetic
// @meta synopsis: x86add.wu rj, rk
// @meta desc: Compute EFLAGS from 32-bit unsigned ADD of rj and rk. OF=0 (unsigned).
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
    uint32_t lhs = (uint32_t)GPR[rj];
    uint32_t rhs = (uint32_t)GPR[rk];
    uint32_t result = lhs + rhs;
    EFLAGS.CF = (uint64_t)lhs + (uint64_t)rhs > UINT32_MAX;
    EFLAGS.AF = ((lhs ^ rhs ^ result) & 0x10) != 0;
    EFLAGS.OF = 0;
    EFLAGS.PF = parity_even((uint8_t)result);
    EFLAGS.ZF = result == 0;
    EFLAGS.SF = (int32_t)result < 0;
    // GPR[rj] and GPR[rk] are unchanged.
}
