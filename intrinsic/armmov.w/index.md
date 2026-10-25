# armmov.w

LBT intrinsic · group: [ARM Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/arm_misc/index.md)

### Synopsis

```
Instruction: armmov.w rj
CPU Flags: LBT
```

### Description

ARM-style move (with flag update): conditionally test the 32-bit value in `rj` and update ARMFLAGS (N, Z). Only ARMFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t r = a;
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
