# \_\_lasx_xvfrstpi_h

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

### Synopsis

```
__m256i __lasx_xvfrstpi_h (__m256i a, __m256i b, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvfrstpi.h xr, xr, imm
CPU Flags: LASX
```

### Description

For each 128-bit lane, find the first negative 16-bit element in `b`. Write its index into the lane of `a` selected by `imm` modulo 8, or write the maximum valid index plus one if no such element exists. Other elements of `a` remain unchanged.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = a.half[i];
}
int i;
for (i = 0; i < 8; i++) {
  if ((s16)b.half[i] < 0) {
    break;
  }
}
dst.half[imm % 8] = i;
for (i = 8; i < 16; i++) {
  if ((s16)b.half[i] < 0) {
    break;
  }
}
dst.half[(imm % 8) + 8] = i - 8;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
