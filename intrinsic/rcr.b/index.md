# rcr.b

LBT intrinsic · group: [LBT Integer](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/integer/index.md)

### Synopsis

```
Instruction: rcr.b rd, rj, rk
CPU Flags: LBT
```

### Description

Rotate 8-bit value in `rj` and CF (in EFLAGS) together as a 9-bit ring right by the amount specified in `rk`. The result is written to `rd`.

### Operation

```
uint8_t v = a;
// Mask count to 5 bits (matching x86 CL masking), then modulo (bits+1)
// for the 9-bit [CF, v] ring rotation.
unsigned c = ((unsigned)b & 0x1F) % (8 + 1);
// Rotate [CF, v] right by c, write low 8 bits to rd
uint64_t ring_hi = EFLAGS.CF;
uint64_t ring_lo = v;
uint64_t r;
if (c == 0) {
  r = v;
} else {
  uint64_t tmp_hi = ring_hi, tmp_lo = ring_lo;
  for (unsigned i = 0; i < c; i++) {
    uint64_t new_lo = (tmp_lo >> 1) | (tmp_hi << (8 - 1));
    uint64_t new_hi = tmp_lo & 1;
    tmp_lo = new_lo;
    tmp_hi = new_hi;
  }
  r = tmp_lo & ((1ULL << 8) - 1);
}
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
