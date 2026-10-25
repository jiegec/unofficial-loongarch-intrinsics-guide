# rotr.w

LBT intrinsic · group: [LBT Integer](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/integer/index.md)

### Synopsis

```
Instruction: rotr.w rd, rj, rk
CPU Flags: LBT
```

### Description

Rotate 32-bit value in `rj` right by the amount specified in `rk`. The shift amount is masked modulo 32.

### Operation

```
uint32_t v = (uint32_t)a;
unsigned c = (unsigned)b % 32;
uint32_t r = c == 0 ? v : (uint32_t)((v >> c) | (v << (32 - c)));
dst = sext(r, 32);
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
