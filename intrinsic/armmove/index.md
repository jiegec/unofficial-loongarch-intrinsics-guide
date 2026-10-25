# armmove

LBT intrinsic · group: [ARM Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/arm_misc/index.md)

### Synopsis

```
Instruction: armmove rd, rj, cond
CPU Flags: LBT
```

### Description

ARM-style conditional move: if the ARM condition holds, copy the value in `rj` to `rd`. ARMFLAGS are not updated.

### Operation

```
if (arm_cond_holds(ARMFLAGS, cond)) {
  dst = a;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 1                |
| 3C6000 | LA664 | 2       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
