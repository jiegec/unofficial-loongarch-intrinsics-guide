# x86sub.b

LBT intrinsic · group: [x86 Integer](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/x86_integer/index.md)

### Synopsis

```
Instruction: x86sub.b rj, rk
CPU Flags: LBT
```

### Description

x86-style subtract: subtract 8-bit values in `rj` and `rk`. Update EFLAGS (CF, AF, OF, PF, ZF, SF) according to the result. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint8_t lhs = (uint8_t)a;
uint8_t rhs = (uint8_t)b;
uint8_t result = lhs - rhs;
EFLAGS.CF = lhs < rhs;
EFLAGS.AF = ((lhs ^ rhs ^ result) & 0x10) != 0;
EFLAGS.OF = ((lhs ^ rhs) & (lhs ^ result) & 0x80) != 0;
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
