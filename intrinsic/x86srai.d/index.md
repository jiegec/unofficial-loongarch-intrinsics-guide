# x86srai.d

LBT intrinsic · group: [x86 Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/x86_shift/index.md)

### Synopsis

```
Instruction: x86srai.d rj, imm
CPU Flags: LBT
```

### Description

x86-style shift right arithmetic: shift 64-bit value in `rj` right arithmetically by immediate `imm`. If shift count is non-zero, update CF, PF, ZF, SF (OF is set to 0 if count is 1). If shift count is zero, flags are unchanged. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint64_t v = (uint64_t)a;
unsigned c = (unsigned)imm;
if (c != 0) {
  uint8_t carry_out = c > 64 ? ((int64_t)v < 0) : ((v >> (c - 1)) & 1);
  uint64_t r = c >= 64 ? (uint64_t)((int64_t)v >> (64 - 1))
                       : (uint64_t)((int64_t)v >> c);
  EFLAGS.CF = carry_out;
  EFLAGS.PF = parity_even((uint8_t)r);
  EFLAGS.ZF = r == 0;
  EFLAGS.SF = (int64_t)r < 0;
  if (c == 1) {
    EFLAGS.OF = 0;
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
