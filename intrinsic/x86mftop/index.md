# x86mftop

LBT intrinsic · group: [x86 FPU](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/x86_fpu/index.md)

### Synopsis

```
Instruction: x86mftop rd
CPU Flags: LBT
```

### Description

Move from x87 FPU stack top pointer: read the current x87 TOP (stack top index, 3-bit) into `rd`.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | N/A     | 4                |
| 3C6000 | LA664 | N/A     | 4                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
