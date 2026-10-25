# \_\_lsx_vshuf4i_d

LSX intrinsic · group: [Shuffling](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/shuffling/index.md)

### Synopsis

```
__m128i __lsx_vshuf4i_d (__m128i a, __m128i b, imm0_255 imm)
#include <lsxintrin.h>
Instruction: vshuf4i.d vr, vr, imm
CPU Flags: LSX
```

### Description

For each 128-bit lane, select two 64-bit elements from the four elements formed by the corresponding lanes of `a` and `b` using `imm[1:0]` and `imm[3:2]`, save the result to `dst`.

### Examples

```
__m128i __lsx_vshuf4i_d(__m128i{0x1122334455667788, 0x99aabbccddeeff00}, __m128i{0xabcdef1314156678, 0x1234123443214321}, 0x12)
= 0xabcdef1314156678 0x1122334455667788
```

### Operation

```
dst.dword[0] = (imm & 2) ? b.dword[(imm & 1)] : a.dword[(imm & 1)];
dst.dword[1] =
    (imm & 8) ? b.dword[((imm >> 2) & 1)] : a.dword[((imm >> 2) & 1)];
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
