# x86rcri.w

LBT intrinsic · group: [x86 Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/x86_shift/index.md)

### Synopsis

```
Instruction: x86rcri.w rj, imm
CPU Flags: LBT
```

### Description

x86-style rotate right through carry: rotate 32-bit value in `rj` and CF together as a 33-bit ring right by immediate `imm`. Update CF, OF (for shift count of 1). Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint32_t v = (uint32_t)a;
unsigned c = (unsigned)(imm & 0x1f);
unsigned n = c;
if (n != 0) {
  unsigned carry_in = EFLAGS.CF;
  unsigned carry_out = ((v >> ((n - 1) % (32 + 1))) & 1);
  EFLAGS.CF = carry_out;
  if (c == 1) {
    uint32_t r = (uint32_t)((v >> 1) | (carry_in << 31));
    EFLAGS.OF = (((r >> 31) ^ (r >> 30)) & 1) != 0;
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
