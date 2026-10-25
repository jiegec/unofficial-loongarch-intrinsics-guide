# \_\_lasx_xvpcnt_h

LASX intrinsic · group: [Bitwise Operations](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/bitwise_operations/index.md)

### Synopsis

```
__m256i __lasx_xvpcnt_h (__m256i a)
#include <lasxintrin.h>
Instruction: xvpcnt.h xr, xr
CPU Flags: LASX
```

### Description

Count the number of ones (population, popcount) in 16-bit elements in `a`.

### Examples

```
__m256i __lasx_xvpcnt_h(__m256i{ 0x1122334455667788, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee})
= 0x0004000600080008 0x0008000a000c0008 0x000a000900050005 0x000a000a000c000c
```

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = popcount(a.half[i]);
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
