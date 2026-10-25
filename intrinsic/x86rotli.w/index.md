# x86rotli.w

LBT intrinsic · group: [x86 Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/x86_shift/index.md)

### Synopsis

```
Instruction: x86rotli.w rj, imm
CPU Flags: LBT
```

### Description

x86-style rotate left: rotate 32-bit value in `rj` left by immediate `imm`. Update CF, OF (for shift count of 1). Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint32_t v = (uint32_t)a;
unsigned c = (unsigned)imm;
unsigned n = c % 32;
uint32_t r = (uint32_t)((v << n) | (v >> (32 - n)));
EFLAGS.CF = ((v >> ((32 - n) % 32)) & 1);
if (c == 1) {
  EFLAGS.OF = ((v ^ r) & 0x80000000) != 0;
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
