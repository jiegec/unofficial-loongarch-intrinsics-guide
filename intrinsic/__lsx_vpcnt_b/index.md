# \_\_lsx_vpcnt_b

LSX intrinsic · group: [Bitwise Operations](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/bitwise_operations/index.md)

### Synopsis

```
__m128i __lsx_vpcnt_b (__m128i a)
#include <lsxintrin.h>
Instruction: vpcnt.b vr, vr
CPU Flags: LSX
```

### Description

Count the number of ones (population, popcount) in 8-bit elements in `a`.

### Examples

```
__m128i __lsx_vpcnt_b(__m128i{0x1122334455667788, 0x99aabbccddeeff00})
= 0x0202040204040602 0x0404060406060800
```

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = popcount(a.byte[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
