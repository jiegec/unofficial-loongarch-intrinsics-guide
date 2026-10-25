# armmfflag

LBT intrinsic · group: [LBT State Transfer](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/state/index.md)

### Synopsis

```
Instruction: armmfflag rd, mask
CPU Flags: LBT
```

### Description

Move from ARMFLAGS: read selected ARM flags (N, Z, C, V) from ARMFLAGS as determined by `mask`, pack them into `rd`.

### Operation

```
dst = 0;
if (mask & (1 << 4))
  dst |= (uint64_t)ARMFLAGS.N << 31;
if (mask & (1 << 3))
  dst |= (uint64_t)ARMFLAGS.Z << 30;
if (mask & (1 << 0))
  dst |= (uint64_t)ARMFLAGS.C << 29;
if (mask & (1 << 5))
  dst |= (uint64_t)ARMFLAGS.V << 28;
dst = sext(dst, 32);
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | N/A     | 1                |
| 3C6000 | LA664 | N/A     | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
