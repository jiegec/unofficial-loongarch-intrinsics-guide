# \_\_lasx_xvpcnt_w

LASX intrinsic · group: [Bitwise Operations](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/bitwise_operations/index.md)

### Synopsis

```
__m256i __lasx_xvpcnt_w (__m256i a)
#include <lasxintrin.h>
Instruction: xvpcnt.w xr, xr
CPU Flags: LASX
```

### Description

Count the number of ones (population, popcount) in 32-bit elements in `a`.

### Examples

```
__m256i __lasx_xvpcnt_w(__m256i{ 0x1122334455667788, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee})
= 0x0000000a00000010 0x0000001200000014 0x000000130000000a 0x0000001400000018
```

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = popcount(a.word[i]);
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
