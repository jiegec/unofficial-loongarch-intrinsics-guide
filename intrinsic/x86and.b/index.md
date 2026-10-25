# x86and.b

LBT intrinsic · group: [x86 Logical](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/x86_logical/index.md)

### Synopsis

```
Instruction: x86and.b rj, rk
CPU Flags: LBT
```

### Description

x86-style bitwise AND: compute 8-bit AND of values in `rj` and `rk`. Clear CF, OF, AF. Update PF, ZF, SF according to the result. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint8_t lhs = (uint8_t)a;
uint8_t rhs = (uint8_t)b;
uint8_t result = lhs & rhs;
EFLAGS.CF = 0;
EFLAGS.OF = 0;
EFLAGS.AF = 0;
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
