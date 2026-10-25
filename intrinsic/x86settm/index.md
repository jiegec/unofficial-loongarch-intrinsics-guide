# x86settm

LBT intrinsic · group: [x86 FPU](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/x86_fpu/index.md)

### Synopsis

```
Instruction: x86settm
CPU Flags: LBT
```

### Description

Set x87 FPU stack translation mode: enable TOP-based remapping of FPR registers (bits 0-7 are offset by TOP when TM is set).

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | N/A     | 0.14(1/7)        |
| 3C6000 | LA664 | N/A     | 0.14(1/7)        |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
