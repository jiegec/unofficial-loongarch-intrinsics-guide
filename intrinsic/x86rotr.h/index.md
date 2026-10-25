# x86rotr.h

LBT intrinsic · group: [x86 Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/x86_shift/index.md)

### Synopsis

```
Instruction: x86rotr.h rj, rk
CPU Flags: LBT
```

### Description

x86-style rotate right: rotate 16-bit value in `rj` right by the amount specified in `rk`. Update CF, OF (for shift count of 1). Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint16_t v = (uint16_t)a;
unsigned c = (unsigned)(b & 0x1f);
unsigned n = c % 16;
uint16_t r = (uint16_t)((v >> n) | (v << (16 - n)));
EFLAGS.CF = ((v >> ((n - 1 + 16) % 16)) & 1);
if (c == 1) {
  EFLAGS.OF = (((v ^ r) & 0x8000) != 0);
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
