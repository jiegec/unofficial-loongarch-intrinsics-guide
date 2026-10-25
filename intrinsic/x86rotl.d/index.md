# x86rotl.d

LBT intrinsic · group: [x86 Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/x86_shift/index.md)

### Synopsis

```
Instruction: x86rotl.d rj, rk
CPU Flags: LBT
```

### Description

x86-style rotate left: rotate 64-bit value in `rj` left by the amount specified in `rk`. Update CF, OF (for shift count of 1). Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint64_t v = (uint64_t)a;
unsigned c = (unsigned)(b & 0x3f);
unsigned n = c % 64;
uint64_t r = (uint64_t)((v << n) | (v >> (64 - n)));
EFLAGS.CF = ((v >> ((64 - n) % 64)) & 1);
if (c == 1) {
  EFLAGS.OF = ((v ^ r) & 0x8000000000000000ULL) != 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
