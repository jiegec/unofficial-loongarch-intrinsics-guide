// fcvt.ld.d
// @meta category: x87_fpu
// @meta synopsis: fcvt.ld.d fd, fj
// @meta desc: Convert 64-bit double in FPR[fj] to x87 extended precision, store lower 64 bits (integer bit + 63-bit fraction) in FPR[fd].
//
// @reg read: FPR[fj]
// @reg write: FPR[fd]
// @reg preserve: all other FPR, all GPR, SCRATCH, EFLAGS, ARMFLAGS
//
// @flag: none
// @cond: none
//
{
    uint64_t d = FPR[rj];
    unsigned exp_d = (d >> 52) & 0x7FF;
    uint64_t man_d = d & 0x000FFFFFFFFFFFFFULL;

    // Integer bit (J): 1 for normal (implied leading 1), 1 for infinity/NaN,
    // 0 for zero (both exponent fields are zero).
    unsigned j_bit;
    uint64_t man_x87;
    if (exp_d == 0 && man_d == 0) {
        j_bit = 0;
        man_x87 = 0;
    } else if (exp_d == 0) {
        // Denormal double: normalize mantissa so leading 1 becomes J bit.
        // Find leading bit position p, shift mantissa left by (63 - p).
        int p = 63 - __builtin_clzll(man_d);
        j_bit = 1;
        man_x87 = (man_d << (63 - p)) & 0x7FFFFFFFFFFFFFFFULL;
    } else {
        // Normal, infinity, NaN: explicit J=1, shift 52-bit fraction to 63-bit.
        j_bit = 1;
        man_x87 = man_d << 11;
        // IEEE 754: quiet signaling NaN during format conversion.
        if (man_d > 0)
            man_x87 |= 1ULL << 62;
    }

    // Lower 64 bits: J[63] | fraction[62:0]
    FPR[rd] = ((uint64_t)j_bit << 63) | man_x87;
}
