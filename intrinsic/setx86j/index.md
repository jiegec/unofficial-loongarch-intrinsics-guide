# setx86j

LBT intrinsic · group: [x86 Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/x86_misc/index.md)

### Synopsis

```
Instruction: setx86j rd, cond
CPU Flags: LBT
```

### Description

x86-style set if condition: evaluate x86 condition code `cond` against EFLAGS. Store 1 in `rd` if the condition holds, 0 otherwise.

### Operation

```
bool res = x86_cond_holds(EFLAGS, cond);
dst = res ? 1 : 0;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | N/A     | 1                |
| 3C6000 | LA664 | N/A     | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
