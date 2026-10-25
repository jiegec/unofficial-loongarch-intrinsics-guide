# addu12i.d

LBT intrinsic · group: [LBT Integer](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/integer/index.md)

### Synopsis

```
Instruction: addu12i.d rd, rj, imm
CPU Flags: LBT
```

### Description

Add sign-extended 5-bit immediate left-shifted by 12 to 64-bit value in `rj` and store the result in `rd`.

### Operation

```
int64_t offset = (int64_t)sext(imm & 0x1f, 5) << 12;
dst = a + (uint64_t)offset;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
