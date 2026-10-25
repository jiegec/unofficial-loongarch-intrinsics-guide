# x86sbc.w

LBT intrinsic · group: [x86 Integer](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/x86_integer/index.md)

### Synopsis

```
Instruction: x86sbc.w rj, rk
CPU Flags: LBT
```

### Description

x86-style subtract with borrow: subtract 32-bit values in `rj` and `rk` with CF (in EFLAGS). Update EFLAGS (CF, AF, OF, PF, ZF, SF) according to the result. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint32_t lhs = (uint32_t)a;
uint32_t rhs = (uint32_t)b;
uint8_t carry_in = EFLAGS.CF;
uint64_t subtrahend = (uint64_t)rhs + carry_in;
uint32_t result = (uint32_t)(lhs - subtrahend);
EFLAGS.CF = lhs < subtrahend;
EFLAGS.AF = (lhs & 0xf) < (rhs & 0xf);
EFLAGS.OF = ((lhs ^ rhs) & (lhs ^ result) & 0x80000000) != 0;
EFLAGS.PF = parity_even((uint8_t)result);
EFLAGS.ZF = result == 0;
EFLAGS.SF = (int32_t)result < 0;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
