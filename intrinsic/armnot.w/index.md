# armnot.w

LBT intrinsic · group: [ARM Logical](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/arm_logical/index.md)

### Synopsis

```
Instruction: armnot.w rj, cond
CPU Flags: LBT
```

### Description

ARM-style bitwise NOT (32-bit): conditionally bitwise NOT the value in `rj`. Update ARMFLAGS (N, Z). Only ARMFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t r = ~(uint32_t)a;
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
