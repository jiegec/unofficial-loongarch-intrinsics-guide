# x86adc.b

LBT intrinsic · group: [x86 Integer](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/x86_integer/index.md)

### Synopsis

```
Instruction: x86adc.b rj, rk
CPU Flags: LBT
```

### Description

x86-style add with carry: add 8-bit values in `rj` and `rk` with CF (in EFLAGS). Update EFLAGS (CF, AF, OF, PF, ZF, SF) according to the result. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint8_t lhs = (uint8_t)a;
uint8_t rhs = (uint8_t)b;
uint8_t carry_in = EFLAGS.CF;
uint8_t result = lhs + rhs + carry_in;
EFLAGS.CF = (uint64_t)lhs + rhs + carry_in > UINT8_MAX;
EFLAGS.AF = ((lhs ^ rhs ^ result) & 0x10) != 0;
EFLAGS.OF = ((~(lhs ^ rhs)) & (lhs ^ result) & 0x80) != 0;
EFLAGS.PF = parity_even((uint8_t)result);
EFLAGS.ZF = result == 0;
EFLAGS.SF = (int8_t)result < 0;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
