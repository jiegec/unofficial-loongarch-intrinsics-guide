# x86rcli.h

LBT intrinsic · group: [x86 Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/x86_shift/index.md)

### Synopsis

```
Instruction: x86rcli.h rj, imm
CPU Flags: LBT
```

### Description

x86-style rotate left through carry: rotate 16-bit value in `rj` and CF together as a 17-bit ring left by immediate `imm`. Update CF, OF (for shift count of 1). Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint16_t v = (uint16_t)a;
unsigned c = (unsigned)(imm & 0x1f);
unsigned n = c % 17;
if (n != 0) {
  unsigned carry_out = ((v >> (16 - n)) & 1);
  EFLAGS.CF = carry_out;
  if (c == 1) {
    uint16_t r = (uint16_t)(v << 1);
    EFLAGS.OF = ((r & 0x8000) != 0) != carry_out;
  }
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
