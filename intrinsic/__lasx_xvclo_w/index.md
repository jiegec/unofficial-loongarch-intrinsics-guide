# \_\_lasx_xvclo_w

LASX intrinsic · group: [Bitwise Operations](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/bitwise_operations/index.md)

### Synopsis

```
__m256i __lasx_xvclo_w (__m256i a)
#include <lasxintrin.h>
Instruction: xvclo.w xr, xr
CPU Flags: LASX
```

### Description

Count leading ones of 32-bit elements in `a`.

### Examples

```
__m256i __lasx_xvclo_w(__m256i{ 0x1122334455667788, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee})
= 0x0000000000000000 0x0000000100000002 0x0000000100000000 0x0000000100000002
```

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = clo(a.word[i]);
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
