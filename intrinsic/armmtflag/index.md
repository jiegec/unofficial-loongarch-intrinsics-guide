# armmtflag

LBT intrinsic · group: [LBT State Transfer](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/state/index.md)

### Synopsis

```
Instruction: armmtflag rd, mask
CPU Flags: LBT
```

### Description

Move to ARMFLAGS: write selected ARM flags (N, Z, C, V) in ARMFLAGS from the corresponding bit positions in `rd`, as determined by `mask`.

### Operation

```
uint64_t v = a;
if (mask & (1 << 4))
  ARMFLAGS.N = (v >> 31) & 1;
if (mask & (1 << 3))
  ARMFLAGS.Z = (v >> 30) & 1;
if (mask & (1 << 0))
  ARMFLAGS.C = (v >> 29) & 1;
if (mask & (1 << 5))
  ARMFLAGS.V = (v >> 28) & 1;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | N/A     | 0.5(1/2)         |
| 3C6000 | LA664 | N/A     | 0.5(1/2)         |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
