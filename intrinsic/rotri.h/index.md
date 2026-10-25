# rotri.h

LBT intrinsic · group: [LBT Integer](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/integer/index.md)

### Synopsis

```
Instruction: rotri.h rd, rj, imm
CPU Flags: LBT
```

### Description

Rotate 16-bit value in `rj` right by immediate `imm`. The shift amount is masked modulo 16.

### Operation

```
uint16_t v = (uint16_t)a;
unsigned c = imm % 16;
uint16_t r = c == 0 ? v : (uint16_t)((v >> c) | (v << (16 - c)));
dst = sext(r, 16);
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 1                |
| 3C6000 | LA664 | 2       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
