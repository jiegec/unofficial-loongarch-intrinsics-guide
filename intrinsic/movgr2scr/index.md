# movgr2scr

LBT intrinsic · group: [LBT State Transfer](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/state/index.md)

### Synopsis

```
Instruction: movgr2scr scrd, rj
CPU Flags: LBT
```

### Description

Move GPR to scratch register: copy value in `rj` to the scratch register selected by `scrd` (0-3). Scratch registers LBT0-LBT3 are accessible via $scr0-$scr3.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | N/A     | 0.5(1/2)         |
| 3C6000 | LA664 | N/A     | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
