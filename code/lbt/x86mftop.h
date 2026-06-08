// x86mftop
// @meta category: overview
// @meta synopsis: x86mftop rd
// @meta desc: rd = x87 TOP (3-bit). Flags unchanged.
//
// @reg read: X87_TOP
// @reg write: GPR[rd]
//
// @flag: none
// @cond: none
//
{
    GPR[rd] = X87_TOP;
    // No flags affected.
}
