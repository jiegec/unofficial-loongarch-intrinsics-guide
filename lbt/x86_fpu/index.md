# x86 FPU

## x86mftop

### Synopsis

```
Instruction: x86mftop rd
CPU Flags: LBT
```

### Description

Move from x87 FPU stack top pointer: read the current x87 TOP (stack top index, 3-bit) into `rd`.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | N/A     | 4                |
| 3C6000 | LA664 | N/A     | 4                |

## x86mttop

### Synopsis

```
Instruction: x86mttop imm
CPU Flags: LBT
```

### Description

Move to x87 FPU stack top pointer: set the x87 TOP (stack top index, 3-bit) to `imm`.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | N/A     | 4                |
| 3C6000 | LA664 | N/A     | 1                |

## x86inctop

### Synopsis

```
Instruction: x86inctop
CPU Flags: LBT
```

### Description

Increment x87 FPU stack top pointer (FINCSTP): increment TOP by 1, wrapping modulo 8.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | N/A     | 4                |
| 3C6000 | LA664 | N/A     | 1                |

## x86dectop

### Synopsis

```
Instruction: x86dectop
CPU Flags: LBT
```

### Description

Decrement x87 FPU stack top pointer (FDECSTP): decrement TOP by 1, wrapping modulo 8.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | N/A     | 4                |
| 3C6000 | LA664 | N/A     | 1                |

## x86settm

### Synopsis

```
Instruction: x86settm
CPU Flags: LBT
```

### Description

Set x87 FPU stack translation mode: enable TOP-based remapping of FPR registers (bits 0-7 are offset by TOP when TM is set).

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | N/A     | 0.14(1/7)        |
| 3C6000 | LA664 | N/A     | 0.14(1/7)        |

## x86clrtm

### Synopsis

```
Instruction: x86clrtm
CPU Flags: LBT
```

### Description

Clear x87 FPU stack translation mode: disable TOP-based remapping of FPR registers.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | N/A     | 0.14(1/7)        |
| 3C6000 | LA664 | N/A     | 0.14(1/7)        |

## x86settag

### Synopsis

```
Instruction: x86settag rd, imm1, imm2
CPU Flags: LBT
```

### Description

Set x87 tag word bit in `rd` according to `imm1` and `imm2`. `imm2` selects the target bit position. `imm1%8` determines the operation: 0=set bit (0→1 only), 1=clear bit (1→0 only), 2-4=check tag byte then modify. Raises a BTE (Binary Translation Exception) on invalid state transitions.

## fcvt.ud.d

### Synopsis

```
Instruction: fcvt.ud.d fd, fj
CPU Flags: LBT
```

### Description

Convert double-precision floating point value in `fj` to the upper 16 bits (sign and exponent) of x87 80-bit extended precision format, store in `fd`.

### Operation

```
uint64_t d = a;
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

// 16 bits: sign[15] | exponent[14:0]
dst = ((uint64_t)sign << 15) | (exp_x87 & 0x7FFF);
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## fcvt.ld.d

### Synopsis

```
Instruction: fcvt.ld.d fd, fj
CPU Flags: LBT
```

### Description

Convert double-precision floating point value in `fj` to the lower 64 bits (integer bit and fraction) of x87 80-bit extended precision format, store in `fd`.

### Operation

```
uint64_t d = a;
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
} else if (exp_d == 0x7FF && man_d == 0) {
  // Infinity: J=1, fraction=0.
  j_bit = 1;
  man_x87 = 0;
} else if (exp_d == 0x7FF) {
  // NaN: map 52-bit double mantissa to 63-bit x87 fraction.
  // Hardware converts SNaN to QNaN on format conversion: forces both
  // J_bit=1 and fraction bit 62 to 1.
  j_bit = 1;
  man_x87 = (man_d << 11) | (1ULL << 62);
} else {
  // Normal: explicit J=1, shift 52-bit fraction to 63-bit.
  j_bit = 1;
  man_x87 = man_d << 11;
}

// Lower 64 bits: J[63] | fraction[62:0]
dst = ((uint64_t)j_bit << 63) | man_x87;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## fcvt.d.ld

### Synopsis

```
Instruction: fcvt.d.ld fd, fj, fk
CPU Flags: LBT
```

### Description

Convert x87 80-bit extended precision value (upper 16 bits in `fj`, lower 64 bits in `fk`) to double-precision floating point, store in `fd`.

### Operation

```
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
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3C6000 | LA664 | 4       | 4                |
