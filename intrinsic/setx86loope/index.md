# setx86loope

LBT intrinsic · group: [x86 Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/x86_misc/index.md)

### Synopsis

```
Instruction: setx86loope rd, rj
CPU Flags: LBT
```

### Description

x86-style loop while equal: the value in `rj` is the counter after decrement. Store 1 in `rd` if the counter is non-zero and ZF (in EFLAGS) is set, 0 otherwise.

### Operation

```
uint64_t counter_after_dec = a;
dst = (counter_after_dec != 0 && EFLAGS.ZF) ? 1 : 0;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 1                |
| 3C6000 | LA664 | 1       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
