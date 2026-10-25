# x86mul.wu

LBT intrinsic · group: [x86 Integer](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/x86_integer/index.md)

### Synopsis

```
Instruction: x86mul.wu rj, rk
CPU Flags: LBT
```

### Description

x86-style multiply: multiply unsigned 32-bit values in `rj` and `rk`. Set CF and OF if overflow (result does not fit in 32-bits), clear other flags. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint32_t lhs = (uint32_t)a;
uint32_t rhs = (uint32_t)b;
unsigned __int128 product =
    (unsigned __int128)(uint32_t)lhs * (unsigned __int128)(uint32_t)rhs;
bool overflow = (product >> 32) != 0;
EFLAGS.CF = overflow;
EFLAGS.OF = overflow;
EFLAGS.SF = 0;
EFLAGS.ZF = 0;
EFLAGS.AF = 0;
EFLAGS.PF = 0;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1.17    | 0.86(1/1.17)     |
| 3C6000 | LA664 | 1       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
