# \_\_lasx_xvpcnt_d

LASX intrinsic · group: [Bitwise Operations](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/bitwise_operations/index.md)

### Synopsis

```
__m256i __lasx_xvpcnt_d (__m256i a)
#include <lasxintrin.h>
Instruction: xvpcnt.d xr, xr
CPU Flags: LASX
```

### Description

Count the number of ones (population, popcount) in 64-bit elements in `a`.

### Examples

```
__m256i __lasx_xvpcnt_d(__m256i{ 0x1122334455667788, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee})
= 0x000000000000001a 0x0000000000000026 0x000000000000001d 0x000000000000002c
```

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = popcount(a.dword[i]);
}
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
