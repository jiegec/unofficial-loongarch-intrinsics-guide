# x86mfflag

LBT intrinsic · group: [LBT State Transfer](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/state/index.md)

### Synopsis

```
Instruction: x86mfflag rd, mask
CPU Flags: LBT
```

### Description

Move from x86 EFLAGS: read selected x86 flags (CF, PF, AF, ZF, SF, OF) from EFLAGS as determined by `mask`, pack them into `rd`.

### Operation

```
dst = 0;
if (mask & (1 << 0))
  dst |= (uint64_t)EFLAGS.CF << 0;
if (mask & (1 << 1))
  dst |= (uint64_t)EFLAGS.PF << 2;
if (mask & (1 << 2))
  dst |= (uint64_t)EFLAGS.AF << 4;
if (mask & (1 << 3))
  dst |= (uint64_t)EFLAGS.ZF << 6;
if (mask & (1 << 4))
  dst |= (uint64_t)EFLAGS.SF << 7;
if (mask & (1 << 5))
  dst |= (uint64_t)EFLAGS.OF << 11;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | N/A     | 1                |
| 3C6000 | LA664 | N/A     | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
