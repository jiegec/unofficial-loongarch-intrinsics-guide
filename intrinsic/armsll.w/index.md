# armsll.w

LBT intrinsic · group: [ARM Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/arm_shift/index.md)

### Synopsis

```
Instruction: armsll.w rj, rk, cond
CPU Flags: LBT
```

### Description

ARM-style logical shift left (32-bit): conditionally shift value in `rj` left by amount in `rk` (masked to 8 bits). Update ARMFLAGS (C, N, Z). Only ARMFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t lhs = (uint32_t)a;
  uint32_t c = (uint32_t)b & 0xff;
  uint32_t r;
  if (c == 0) {
    r = lhs;
  } else if (c < 32) {
    ARMFLAGS.C = (lhs >> (32 - c)) & 1;
    r = lhs << c;
  } else if (c == 32) {
    ARMFLAGS.C = lhs & 1;
    r = 0;
  } else {
    ARMFLAGS.C = 0;
    r = 0;
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
