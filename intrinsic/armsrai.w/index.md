# armsrai.w

LBT intrinsic · group: [ARM Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/arm_shift/index.md)

### Synopsis

```
Instruction: armsrai.w rj, imm, cond
CPU Flags: LBT
```

### Description

ARM-style arithmetic shift right immediate (32-bit): conditionally shift value in `rj` right arithmetically by `imm`. Update ARMFLAGS (C, N, Z). Only ARMFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t v = (uint32_t)a;
  uint32_t r = (uint32_t)((int32_t)v >> imm);
  ARMFLAGS.C = imm == 0 ? ARMFLAGS.C : (imm > 32 ? 0 : ((v >> (imm - 1)) & 1));
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
