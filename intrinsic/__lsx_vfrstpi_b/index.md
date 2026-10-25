# \_\_lsx_vfrstpi_b

LSX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/misc/index.md)

### Synopsis

```
__m128i __lsx_vfrstpi_b (__m128i a, __m128i b, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vfrstpi.b vr, vr, imm
CPU Flags: LSX
```

### Description

For each 128-bit lane, find the first negative 8-bit element in `b`. Write its index into the lane of `a` selected by `imm` modulo 16, or write the maximum valid index plus one if no such element exists. Other elements of `a` remain unchanged.

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
dst.byte[imm % 16] = i;
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
