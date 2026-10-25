# rcr.d

LBT intrinsic · group: [LBT Integer](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/integer/index.md)

### Synopsis

```
Instruction: rcr.d rd, rj, rk
CPU Flags: LBT
```

### Description

Rotate 64-bit value in `rj` and CF (in EFLAGS) together as a 65-bit ring right by the amount specified in `rk`. The result is written to `rd`.

### Operation

```
uint64_t v = (uint64_t)a;
unsigned c = (unsigned)b & 0x3f;
// Rotate [CF, v] right by c, write low 64 bits to rd
uint64_t ring_hi = EFLAGS.CF;
uint64_t ring_lo = v;
uint64_t r;
if (c == 0) {
  r = v;
} else {
  uint64_t tmp_hi = ring_hi, tmp_lo = ring_lo;
  for (unsigned i = 0; i < c; i++) {
    uint64_t new_lo = (tmp_lo >> 1) | (tmp_hi << (64 - 1));
    uint64_t new_hi = tmp_lo & 1;
    tmp_lo = new_lo;
    tmp_hi = new_hi;
  }
  r = tmp_lo;
}
dst = r;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 1                |
| 3C6000 | LA664 | 2       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
