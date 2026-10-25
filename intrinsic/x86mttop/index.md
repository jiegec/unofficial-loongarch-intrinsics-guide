# x86mttop

LBT intrinsic · group: [x86 FPU](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/x86_fpu/index.md)

### Synopsis

```
Instruction: x86mttop imm
CPU Flags: LBT
```

### Description

Move to x87 FPU stack top pointer: set the x87 TOP (stack top index, 3-bit) to `imm`.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | N/A     | 4                |
| 3C6000 | LA664 | N/A     | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
