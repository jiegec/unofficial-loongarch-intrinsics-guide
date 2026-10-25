# x86dectop

LBT intrinsic · group: [x86 FPU](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/x86_fpu/index.md)

### Synopsis

```
Instruction: x86dectop
CPU Flags: LBT
```

### Description

Decrement x87 FPU stack top pointer (FDECSTP): decrement TOP by 1, wrapping modulo 8.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | N/A     | 4                |
| 3C6000 | LA664 | N/A     | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
