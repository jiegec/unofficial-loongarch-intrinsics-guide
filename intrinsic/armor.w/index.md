# armor.w

LBT intrinsic · group: [ARM Logical](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/arm_logical/index.md)

### Synopsis

```
Instruction: armor.w rj, rk, cond
CPU Flags: LBT
```

### Description

ARM-style bitwise OR (32-bit): conditionally OR values in `rj` and `rk`. Update ARMFLAGS (N, Z). Only ARMFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t lhs = (uint32_t)a;
  uint32_t rhs = (uint32_t)b;
  uint32_t r = lhs | rhs;
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
