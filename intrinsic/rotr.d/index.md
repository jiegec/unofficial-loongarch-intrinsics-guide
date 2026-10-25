# rotr.d

LBT intrinsic · group: [LBT Integer](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/integer/index.md)

### Synopsis

```
Instruction: rotr.d rd, rj, rk
CPU Flags: LBT
```

### Description

Rotate 64-bit value in `rj` right by the amount specified in `rk`. The shift amount is masked modulo 64.

### Operation

```
uint64_t v = (uint64_t)a;
unsigned c = (unsigned)b % 64;
uint64_t r = c == 0 ? v : (uint64_t)((v >> c) | (v << (64 - c)));
dst = r;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
