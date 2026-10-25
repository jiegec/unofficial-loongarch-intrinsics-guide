# \_\_lasx_xvperm_w

LASX intrinsic · group: [Permutation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/permutation/index.md)

### Synopsis

```
__m256i __lasx_xvperm_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvperm.w xr, xr, xr
CPU Flags: LASX
```

### Description

For each word, use the low 3 bits of the corresponding word in `b` as an index into the eight words of `a`, and store the result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = a.word[b.word[i] % 0x8];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 4                |
| 3C6000 | LA664 | 3       | 4                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
