# \_\_lasx_xvclz_b

LASX intrinsic · group: [Bitwise Operations](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/bitwise_operations/index.md)

### Synopsis

```
__m256i __lasx_xvclz_b (__m256i a)
#include <lasxintrin.h>
Instruction: xvclz.b xr, xr
CPU Flags: LASX
```

### Description

Count leading zeros of 8-bit elements in `a`.

### Examples

```
__m256i __lasx_xvclz_b(__m256i{ 0x1122334455667788, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee})
= 0x0302020101010100 0x0000000000000008 0x0000000303020302 0x0000000000000000
```

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = clz(a.byte[i]);
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
