// x86inctop
// @meta category: overview
// @meta synopsis: x86inctop
// @meta desc: x87 TOP = (TOP + 1) mod 8. Flags unchanged.
//
// @reg read: X87_TOP
// @reg write: X87_TOP
//
// @flag: none
// @cond: none
//
{
    X87_TOP = (X87_TOP + 1) & 0x7;
}
