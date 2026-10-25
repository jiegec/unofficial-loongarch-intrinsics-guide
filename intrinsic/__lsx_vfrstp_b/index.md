# \_\_lsx_vfrstp_b

LSX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/misc/index.md)

### Synopsis

```
__m128i __lsx_vfrstp_b (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vfrstp.b vr, vr, vr
CPU Flags: LSX
```

### Description

For each 128-bit lane, find the first negative 8-bit element in `b`. Write its index into the lane of `a` selected by the first element of the corresponding lane of `c` modulo 16, or write the maximum valid index plus one if no such element exists. Other elements of `a` remain unchanged.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = a.byte[i];
}
int i;
for (i = 0; i < 16; i++) {
  if ((s8)b.byte[i] < 0) {
    break;
  }
}
dst.byte[c.byte[0] % 16] = i;
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 1       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
