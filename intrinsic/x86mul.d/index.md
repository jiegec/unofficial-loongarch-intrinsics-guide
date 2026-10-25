# x86mul.d

LBT intrinsic · group: [x86 Integer](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/x86_integer/index.md)

### Synopsis

```
Instruction: x86mul.d rj, rk
CPU Flags: LBT
```

### Description

x86-style multiply: multiply signed 64-bit values in `rj` and `rk`. Set CF and OF if overflow (result does not fit in 64-bits), clear other flags. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint64_t lhs = (uint64_t)a;
uint64_t rhs = (uint64_t)b;
__int128 product = (__int128)(int64_t)lhs * (__int128)(int64_t)rhs;
bool overflow = product < (__int128)INT64_MIN || product > (__int128)INT64_MAX;
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
