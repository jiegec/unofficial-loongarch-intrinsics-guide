# \_\_lsx_vfscaleb_s

LSX intrinsic · group: [Undocumented Intrinsics](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/undocumented/index.md)

### Synopsis

```
__m128 __lsx_vfscaleb_s (__m128 a, __m128i b)
#include <lsxintrin.h>
Instruction: vfscaleb.s vr, vr, vr
CPU Flags: LSX
```

### Description

Compute IEEE 754 scaleB (multiply by 2^`b`) of single precision floating point elements in `a` using integer exponents in `b`. Currently undocumented.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp32[i] = __builtin_scalbn(a.fp32[i], b.word[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
