# sbc.b

LBT intrinsic · group: [LBT Integer](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/integer/index.md)

### Synopsis

```
Instruction: sbc.b rd, rj, rk
CPU Flags: LBT
```

### Description

Subtract 8-bit values in `rj` and `rk` with borrow (CF in EFLAGS), sign-extend the result to 64-bit and store in `rd`.

### Operation

```
uint8_t lhs = a;
uint8_t rhs = b;
uint8_t cf = EFLAGS.CF;
uint8_t r = lhs - rhs - cf;
dst = sext(r, 8);
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 1                |
| 3C6000 | LA664 | 2       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
