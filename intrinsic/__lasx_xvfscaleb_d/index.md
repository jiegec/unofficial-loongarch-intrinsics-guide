# \_\_lasx_xvfscaleb_d

LASX intrinsic · group: [Undocumented Intrinsics](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/undocumented/index.md)

### Synopsis

```
__m256d __lasx_xvfscaleb_d (__m256d a, __m256i b)
#include <lasxintrin.h>
Instruction: xvfscaleb.d xr, xr, xr
CPU Flags: LASX
```

### Description

Compute IEEE 754 scaleB (multiply by 2^`b`) of double precision floating point elements in `a` using integer exponents in `b`. Currently undocumented.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp64[i] = __builtin_scalbn(a.fp64[i], b.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C6000 | LA664 | 4       | 2                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
