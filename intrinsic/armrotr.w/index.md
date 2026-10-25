# armrotr.w

LBT intrinsic · group: [ARM Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/arm_shift/index.md)

### Synopsis

```
Instruction: armrotr.w rj, rk, cond
CPU Flags: LBT
```

### Description

ARM-style rotate right (32-bit): conditionally rotate value in `rj` right by amount in `rk` (masked to 8 bits, rotate modulo 32). Update ARMFLAGS (C, N, Z). Only ARMFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t lhs = (uint32_t)a;
  uint32_t rhs = (uint32_t)b;
  uint32_t raw_count = rhs & 0xff;
  uint32_t c = rhs & 0x1f;
  unsigned n = c % 32;
  uint32_t r = n == 0 ? lhs : (lhs >> n) | (lhs << (32 - n));
  if (raw_count != 0) {
    ARMFLAGS.C = n == 0 ? ((lhs >> 31) & 1) : ((lhs >> (n - 1)) & 1);
  }
  ARMFLAGS.N = (r >> 31) & 1;
  ARMFLAGS.Z = r == 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
