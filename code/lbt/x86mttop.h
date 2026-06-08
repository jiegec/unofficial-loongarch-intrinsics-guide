// x86mttop
// @meta category: overview
// @meta synopsis: x86mttop top
// @meta desc: x87 TOP = top. Flags unchanged.
//
// @reg read: none (immediate operand)
// @reg write: X87_TOP
//
// @flag: none
// @cond: none
//
{
    X87_TOP = top & 0x7;
    // No flags affected.
}
