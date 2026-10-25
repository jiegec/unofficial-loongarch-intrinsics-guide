# fcvt.ld.d

LBT intrinsic · group: [x86 FPU](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/x86_fpu/index.md)

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

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
