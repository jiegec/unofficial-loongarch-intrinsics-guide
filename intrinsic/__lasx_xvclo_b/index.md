# \_\_lasx_xvclo_b

LASX intrinsic · group: [Bitwise Operations](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/bitwise_operations/index.md)

### Synopsis

```
__m256i __lasx_xvclo_b (__m256i a)
#include <lasxintrin.h>
Instruction: xvclo.b xr, xr
CPU Flags: LASX
```

### Description

Count leading ones of 8-bit elements in `a`.

### Examples

```
__m256i __lasx_xvclo_b(__m256i{ 0x1122334455667788, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee})
= 0x0000000000000001 0x0101010202030800 0x0102030000000000 0x0101010102030203
```

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = clo(a.byte[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
