// x86or.w
// @meta category: x86_logic
// @meta synopsis: x86or.w rj, rk
// @meta desc: Compute EFLAGS from 32-bit OR of rj and rk. CF=OF=0.
//
// @reg read: GPR[rj], GPR[rk]
// @reg write: EFLAGS[CF,PF,AF,ZF,SF,OF]
//
// @flag CF: FORCE_0          always cleared
// @flag PF: COMPUTE          parity of result low byte
// @flag AF: FORCE_0          tested cleared on LA464/LA664
// @flag ZF: COMPUTE          result == 0
// @flag SF: COMPUTE          MSB of result
// @flag OF: FORCE_0          always cleared
// @cond: none
//
{
    uint32_t lhs = (uint32_t)GPR[rj];
    uint32_t rhs = (uint32_t)GPR[rk];
    uint32_t result = lhs | rhs;
    EFLAGS.CF = 0;
    EFLAGS.OF = 0;
    EFLAGS.AF = 0;
    EFLAGS.PF = parity_even((uint8_t)result);
    EFLAGS.ZF = result == 0;
    EFLAGS.SF = (int32_t)result < 0;
    // GPR[rj] and GPR[rk] are unchanged.
}
