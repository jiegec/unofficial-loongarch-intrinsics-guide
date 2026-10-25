# x86slli.w

LBT intrinsic · group: [x86 Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/x86_shift/index.md)

### Synopsis

```
Instruction: x86slli.w rj, imm
CPU Flags: LBT
```

### Description

x86-style shift left logical: shift 32-bit value in `rj` left by immediate `imm`. If shift count is non-zero, update CF, PF, ZF, SF (and OF if count is 1). If shift count is zero, flags are unchanged. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint32_t v = (uint32_t)a;
unsigned c = (unsigned)imm;
if (c != 0) {
  uint8_t carry_out = c > 32 ? 0 : ((v >> (32 - c)) & 1);
  uint32_t r = c >= 32 ? 0 : (uint32_t)(v << c);
  EFLAGS.CF = carry_out;
  EFLAGS.PF = parity_even((uint8_t)r);
  EFLAGS.ZF = r == 0;
  EFLAGS.SF = (int32_t)r < 0;
  if (c == 1) {
    EFLAGS.OF = ((v ^ r) & 0x80000000) != 0;
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
