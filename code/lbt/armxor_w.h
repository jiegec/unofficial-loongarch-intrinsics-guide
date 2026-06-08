// armxor.w
// @meta category: arm
// @meta synopsis: armxor.w rj, rk, cond
// @meta desc: Compute ARM flags from 32-bit XOR of rj and rk. Updates N,Z if cond holds. C,V preserved.
//
// @reg read: GPR[rj], GPR[rk], ARMFLAGS (condition)
// @reg write: ARMFLAGS[N,Z]
//
// @flag N: COMPUTE          sign of result
// @flag Z: COMPUTE          result == 0
// @flag C: PRESERVE         preserved for logical
// @flag V: PRESERVE         preserved for logical
// @cond: cond false => all writes suppressed and state preserved
//
{
    if (!arm_cond_holds(ARMFLAGS, cond))
        return;
    uint32_t lhs = (uint32_t)GPR[rj];
    uint32_t rhs = (uint32_t)GPR[rk];
    uint32_t r = lhs ^ rhs;
    // C, V preserved for logical ARM helpers
    ARMFLAGS.N = (r >> 31) & 1;
    ARMFLAGS.Z = r == 0;
    // C, V preserved
    // GPR operands are unchanged.
}
