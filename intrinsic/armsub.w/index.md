# armsub.w

LBT intrinsic · group: [ARM Integer](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/arm_integer/index.md)

### Synopsis

```
Instruction: armsub.w rj, rk, cond
CPU Flags: LBT
```

### Description

ARM-style subtract (32-bit): conditionally subtract value in `rk` from `rj`. Update ARMFLAGS (C, V, N, Z). Only ARMFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t lhs = (uint32_t)a;
  uint32_t rhs = (uint32_t)b;
  uint64_t subtrahend = (uint64_t)rhs;
  uint32_t r = (uint32_t)(lhs - subtrahend);
  ARMFLAGS.C = lhs < subtrahend;
  ARMFLAGS.V = ((lhs ^ rhs) & (lhs ^ r) & 0x80000000) != 0;
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
