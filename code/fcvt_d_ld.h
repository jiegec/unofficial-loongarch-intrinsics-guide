uint64_t lo = a; // J[63] | fraction[62:0]
uint64_t hi = b; // sign[15] | exponent[14:0]

unsigned sign = (hi >> 15) & 1;
int64_t exp_x87 = hi & 0x7FFF;
unsigned j_bit = (lo >> 63) & 1;
uint64_t man_x87 = lo & 0x7FFFFFFFFFFFFFFFULL;

// Build the full 64-bit significand: J_bit.fraction
uint64_t full = ((uint64_t)j_bit << 63) | man_x87;

// Re-bias exponent from x87 extended (16383) to double (1023).
// Round 63-bit fraction to 52 bits with round-to-nearest-even.
unsigned exp_d;
uint64_t man_d;
if (exp_x87 == 0x7FFF && j_bit == 1) {
  // True infinity or NaN with J=1.  Hardware always produces a
  // quiet NaN by setting bit 51 of the mantissa.
  man_d = man_x87 >> 11;
  man_d |= (1ULL << 51); // ensure QNaN
  if (man_x87 == 0) {
    // x87 infinity (fraction is zero) → double infinity (mantissa zero)
    man_d = 0;
  }
  exp_d = 0x7FF;
} else if (exp_x87 == 0 && j_bit == 0) {
  // Pseudo-denormal or zero: J=0 with zero exponent.  Hardware
  // treats all of these as zero (pseudo-denormals flush to zero).
  exp_d = 0;
  man_d = 0;
} else if (j_bit == 0) {
  // Unnormal or pseudo-NaN: J=0 with non-zero exponent is an
  // invalid x87 encoding.  Hardware produces the default quiet
  // NaN (sign=0, mantissa=0x8000000000000, exponent=0x7FF).
  sign = 0;
  exp_d = 0x7FF;
  man_d = 0x8000000000000ULL;
} else {
  int64_t rebias = exp_x87 - 16383 + 1023;
  if (rebias >= 0x7FF) {
    // Overflow to infinity.
    exp_d = 0x7FF;
    man_d = 0;
  } else if (rebias <= 0) {
    // Underflow: produce double denormal.
    // Right-shift the full 64-bit significand, then RNE to 52 bits.
    int shift = 1 - (int)rebias;
    exp_d = 0;
    int rshift =
        11 + shift; // total right-shift for 64-bit -> denormal mantissa

    // Handle rshift >= 64 separately: all bits discarded.
    if (rshift >= 64) {
      // Result before rounding is zero.  RNE rounds up to the smallest
      // denormal (man_d=1) only when rshift == 64 and full > 2^63
      // (guard=1, sticky=1 -> round up).  Ties (full == 2^63, guard=1,
      // sticky=0, LSB=0) and all rshift > 64 cases round down to 0.
      man_d = (rshift == 64 && full > (1ULL << 63)) ? 1 : 0;
    } else {
      uint64_t result = full >> rshift;
      uint64_t lost = full & ((1ULL << rshift) - 1);
      uint64_t half = 1ULL << (rshift - 1);

      // Round-to-nearest-even on the discarded bits
      if (lost > half || (lost == half && (result & 1))) {
        result++;
      }

      // If rounding overflowed past the denormal boundary, it becomes
      // the smallest normal number (biased exponent 1, mantissa 0).
      if (result & ((uint64_t)1 << 52)) {
        // overflow to smallest normal
        exp_d = 1;
        man_d = 0;
      } else {
        man_d = result & 0x000FFFFFFFFFFFFFULL;
      }
    }
  } else {
    // Normal case: round 64-bit significand to 53 bits, then strip
    // the implicit leading 1 to get the 52-bit double mantissa.
    exp_d = (unsigned)rebias;
    uint64_t result53 = full >> 11;  // top 53 bits (bit 52 = implicit 1)
    uint64_t lost = full & 0x7FFULL; // bottom 11 bits
    uint64_t half = 0x400ULL;

    // Round-to-nearest-even
    if (lost > half || (lost == half && (result53 & 1))) {
      result53++;
      if (result53 & (1ULL << 53)) {
        // Carry into bit 53 → significand wrapped to 2.0.
        // Renormalize and adjust exponent.
        result53 >>= 1;
        exp_d++;
        if (exp_d >= 0x7FF) {
          // Overflowed to infinity.
          exp_d = 0x7FF;
          result53 = 0;
        }
      }
    }
    man_d = result53 & 0x000FFFFFFFFFFFFFULL;
  }
}

// Recombine: sign[63] | exponent[62:52] | fraction[51:0]
dst = ((uint64_t)sign << 63) | ((uint64_t)exp_d << 52) | man_d;