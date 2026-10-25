# \_\_lasx_xvsrani_d_q

LASX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shift/index.md)

### Synopsis

```
__m256i __lasx_xvsrani_d_q (__m256i a, __m256i b, imm0_127 imm)
#include <lasxintrin.h>
Instruction: xvsrani.d.q xr, xr, imm
CPU Flags: LASX
```

### Description

Arithmetic right shift the signed 128-bit elements in `a` and `b` by `imm`, write the 64-bit results into `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (i < 1) ? (s64)((s128)b.qword[i] >> imm)
                         : (s64)((s128)a.qword[i - 1] >> imm);
}
for (int i = 2; i < 4; i++) {
  dst.dword[i] = (i < 3) ? (s64)((s128)b.qword[i - 1] >> imm)
                         : (s64)((s128)a.qword[i - 2] >> imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
