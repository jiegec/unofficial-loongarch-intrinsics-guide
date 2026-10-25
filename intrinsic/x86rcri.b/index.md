# x86rcri.b

LBT intrinsic · group: [x86 Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/x86_shift/index.md)

### Synopsis

```
Instruction: x86rcri.b rj, imm
CPU Flags: LBT
```

### Description

x86-style rotate right through carry: rotate 8-bit value in `rj` and CF together as a 9-bit ring right by immediate `imm`. Update CF, OF (for shift count of 1). Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint8_t v = (uint8_t)a;
unsigned c = (unsigned)(imm & 0x1f);
unsigned n = c % 9;
if (n != 0) {
  unsigned carry_in = EFLAGS.CF;
  unsigned carry_out = ((v >> ((n - 1) % (8 + 1))) & 1);
  EFLAGS.CF = carry_out;
  if (c == 1) {
    uint8_t r = (uint8_t)((v >> 1) | (carry_in << 7));
    EFLAGS.OF = (((r >> 7) ^ (r >> 6)) & 1) != 0;
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
