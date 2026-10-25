# rotr.b

LBT intrinsic · group: [LBT Integer](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/integer/index.md)

### Synopsis

```
Instruction: rotr.b rd, rj, rk
CPU Flags: LBT
```

### Description

Rotate 8-bit value in `rj` right by the amount specified in `rk`. The shift amount is masked modulo 8.

### Operation

```
uint8_t v = (uint8_t)a;
unsigned c = (unsigned)b % 8;
uint8_t r = c == 0 ? v : (uint8_t)((v >> c) | (v << (8 - c)));
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
