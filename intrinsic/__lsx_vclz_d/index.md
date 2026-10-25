# \_\_lsx_vclz_d

LSX intrinsic · group: [Bitwise Operations](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/bitwise_operations/index.md)

### Synopsis

```
__m128i __lsx_vclz_d (__m128i a)
#include <lsxintrin.h>
Instruction: vclz.d vr, vr
CPU Flags: LSX
```

### Description

Count leading zeros of 64-bit elements in `a`.

### Examples

```
__m128i __lsx_vclz_d(__m128i{0x1122334455667788, 0x99aabbccddeeff00})
= 0x0000000000000003 0x0000000000000000
```

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = clz(a.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 1       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
