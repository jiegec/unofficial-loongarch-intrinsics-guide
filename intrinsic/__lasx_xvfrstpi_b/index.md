# \_\_lasx_xvfrstpi_b

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

### Synopsis

```
__m256i __lasx_xvfrstpi_b (__m256i a, __m256i b, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvfrstpi.b xr, xr, imm
CPU Flags: LASX
```

### Description

For each 128-bit lane, find the first negative 8-bit element in `b`. Write its index into the lane of `a` selected by `imm` modulo 16, or write the maximum valid index plus one if no such element exists. Other elements of `a` remain unchanged.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = a.byte[i];
}
int i;
for (i = 0; i < 16; i++) {
  if ((s8)b.byte[i] < 0) {
    break;
  }
}
dst.byte[imm % 16] = i;
for (i = 16; i < 32; i++) {
  if ((s8)b.byte[i] < 0) {
    break;
  }
}
dst.byte[(imm % 16) + 16] = i - 16;
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
