// fcvt.d.ld
// @meta category: x87_fpu
// @meta synopsis: fcvt.d.ld fd, fj, fk
// @meta desc: Recompose 64-bit double in FPR[fd] from x87 extended precision value split across FPR[fj] (low 64 bits: integer bit + 63-bit fraction) and FPR[fk] (high 16 bits: sign + exponent).
//
// @reg read: FPR[fj], FPR[fk]
// @reg write: FPR[fd]
// @reg preserve: all other FPR, all GPR, SCRATCH, EFLAGS, ARMFLAGS
//
// @flag: none
// @cond: none
//
{
    uint64_t lo = FPR[rj];       // J[63] | fraction[62:0]
    uint64_t hi = FPR[rk];       // sign[15] | exponent[14:0]

    unsigned sign = (hi >> 15) & 1;
    int64_t exp_x87 = hi & 0x7FFF;
    unsigned j_bit = (lo >> 63) & 1;
    uint64_t man_x87 = lo & 0x7FFFFFFFFFFFFFFFULL;

    // Re-bias exponent from x87 extended (16383) to double (1023).
    // Round 63-bit fraction to 52 bits (truncated here; hardware
    // may implement round-to-nearest-even).
    unsigned exp_d;
    uint64_t man_d;
    if (exp_x87 == 0x7FFF) {
        // Infinity or NaN.
        exp_d = 0x7FF;
        man_d = man_x87 >> 11;
    } else if (exp_x87 == 0 && j_bit == 0 && man_x87 == 0) {
        // Zero.
        exp_d = 0;
        man_d = 0;
    } else {
        int64_t rebias = exp_x87 - 16383 + 1023;
        if (rebias >= 0x7FF) {
            // Overflow to infinity.
            exp_d = 0x7FF;
            man_d = 0;
        } else if (rebias <= 0) {
            // Underflow: produce double denormal.
            // Shift the full mantissa (with J bit) right to create subnormal.
            int shift = 1 - (int)rebias;
            exp_d = 0;
            if (shift < 64) {
                uint64_t full = ((uint64_t)j_bit << 63) | man_x87;
                man_d = (full >> (11 + shift)) & 0x000FFFFFFFFFFFFFULL;
            } else {
                man_d = 0;
            }
        } else {
            exp_d = (unsigned)rebias;
            man_d = man_x87 >> 11;
        }
    }

    // Recombine: sign[63] | exponent[62:52] | fraction[51:0]
    FPR[rd] = ((uint64_t)sign << 63) | ((uint64_t)exp_d << 52) | man_d;
}
