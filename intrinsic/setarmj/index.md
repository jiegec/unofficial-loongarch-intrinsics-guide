# setarmj

LBT intrinsic · group: [ARM Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/arm_misc/index.md)

### Synopsis

```
Instruction: setarmj rd, cond
CPU Flags: LBT
```

### Description

ARM-style set if condition: evaluate ARM condition code `cond` against ARMFLAGS. Store 1 in `rd` if the condition holds, 0 otherwise.

### Operation

```
bool res = arm_cond_holds(ARMFLAGS, cond);
dst = res ? 1 : 0;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | N/A     | 1                |
| 3C6000 | LA664 | N/A     | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
