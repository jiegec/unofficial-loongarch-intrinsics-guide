# x86mtflag

LBT intrinsic · group: [LBT State Transfer](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/state/index.md)

### Synopsis

```
Instruction: x86mtflag rd, mask
CPU Flags: LBT
```

### Description

Move to x86 EFLAGS: write selected x86 flags (CF, PF, AF, ZF, SF, OF) in EFLAGS from the corresponding bit positions in `rd`, as determined by `mask`.

### Operation

```
uint64_t v = a;
if (mask & (1 << 0))
  EFLAGS.CF = (v >> 0) & 1;
if (mask & (1 << 1))
  EFLAGS.PF = (v >> 2) & 1;
if (mask & (1 << 2))
  EFLAGS.AF = (v >> 4) & 1;
if (mask & (1 << 3))
  EFLAGS.ZF = (v >> 6) & 1;
if (mask & (1 << 4))
  EFLAGS.SF = (v >> 7) & 1;
if (mask & (1 << 5))
  EFLAGS.OF = (v >> 11) & 1;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | N/A     | 0.5(1/2)         |
| 3C6000 | LA664 | N/A     | 0.5(1/2)         |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
