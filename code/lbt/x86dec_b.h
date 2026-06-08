// x86dec.b
// @meta category: x86_arithmetic
// @meta synopsis: x86dec.b rj
// @meta desc: Compute EFLAGS from 8-bit DEC of rj. CF preserved.
//
// @reg read: GPR[rj]
// @reg write: EFLAGS[PF,AF,ZF,SF,OF]
//
// @flag CF: PRESERVE         preserved through
// @flag PF: COMPUTE          parity of result low byte
// @flag AF: COMPUTE          auxiliary carry/borrow
// @flag ZF: COMPUTE          result == 0
// @flag SF: COMPUTE          MSB of result
// @flag OF: COMPUTE          signed overflow
// @cond: none
//
{
    uint8_t v = (uint8_t)GPR[rj];
    uint8_t r = v - 1;
    // CF preserved from input EFLAGS.CF
    EFLAGS.PF = parity_even((uint8_t)r);
    EFLAGS.AF = ((v ^ 1 ^ r) & 0x10) != 0;
    EFLAGS.ZF = r == 0;
    EFLAGS.SF = (int8_t)r < 0;
    EFLAGS.OF = v == (uint8_t)INT8_MIN;
    // GPR[rj] is unchanged.
}
