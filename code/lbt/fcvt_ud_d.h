// fcvt.ud.d
// @meta category: x87_fpu
// @meta synopsis: fcvt.ud.d fd, fj
// @meta desc: Convert 64-bit double in FPR[fj] to x87 extended precision, store upper 16 bits (sign + exponent) zero-extended to 64 bits in FPR[fd].
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
    unsigned sign = (d >> 63) & 1;
    unsigned exp_d = (d >> 52) & 0x7FF;
    uint64_t man_d = d & 0x000FFFFFFFFFFFFFULL;

    // Re-bias exponent from double (1023) to x87 extended (16383).
    // Double uses an implied integer bit (J=1 for normal numbers).
    // x87 extended uses an explicit integer bit.
    int64_t exp_x87;
    if (exp_d == 0x7FF) {
        // Infinity or NaN: x87 exponent is also all-ones.
        exp_x87 = 0x7FFF;
    } else if (exp_d == 0 && man_d == 0) {
        // Signed zero: x87 exponent 0, integer bit 0.
        exp_x87 = 0;
    } else if (exp_d == 0) {
        // Denormal double: value = man_d * 2^(-1074).
        // Normalize by finding leading bit position p (0-indexed from LSB).
        // Resulting x87 exponent: (-1074 + p) biased by 16383.
        int p = 63 - __builtin_clzll(man_d);
        exp_x87 = -1074 + p + 16383;
    } else {
        // Normal double → normal x87 extended.
        exp_x87 = (int64_t)exp_d - 1023 + 16383;
    }

    // Upper 16 bits: sign[15] | exponent[14:0]
    FPR[rd] = ((uint64_t)sign << 15) | (exp_x87 & 0x7FFF);
}
