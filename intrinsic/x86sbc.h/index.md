# x86sbc.h

LBT intrinsic · group: [x86 Integer](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/x86_integer/index.md)

### Synopsis

```
Instruction: x86sbc.h rj, rk
CPU Flags: LBT
```

### Description

x86-style subtract with borrow: subtract 16-bit values in `rj` and `rk` with CF (in EFLAGS). Update EFLAGS (CF, AF, OF, PF, ZF, SF) according to the result. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint16_t lhs = (uint16_t)a;
uint16_t rhs = (uint16_t)b;
uint8_t carry_in = EFLAGS.CF;
uint64_t subtrahend = (uint64_t)rhs + carry_in;
uint16_t result = (uint16_t)(lhs - subtrahend);
EFLAGS.CF = lhs < subtrahend;
EFLAGS.AF = (lhs & 0xf) < (rhs & 0xf);
EFLAGS.OF = ((lhs ^ rhs) & (lhs ^ result) & 0x8000) != 0;
EFLAGS.PF = parity_even((uint8_t)result);
EFLAGS.ZF = result == 0;
EFLAGS.SF = (int16_t)result < 0;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
