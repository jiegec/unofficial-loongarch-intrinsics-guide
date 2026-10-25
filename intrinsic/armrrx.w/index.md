# armrrx.w

LBT intrinsic · group: [ARM Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/arm_shift/index.md)

### Synopsis

```
Instruction: armrrx.w rj, cond
CPU Flags: LBT
```

### Description

ARM-style rotate right extended (32-bit): conditionally rotate the value in `rj` and C (in ARMFLAGS) together as a 33-bit ring right by 1. Update ARMFLAGS (C, N, Z). Only ARMFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t r = ((uint32_t)a >> 1) | (ARMFLAGS.C ? 0x80000000 : 0);
  ARMFLAGS.C = a & 1;
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
