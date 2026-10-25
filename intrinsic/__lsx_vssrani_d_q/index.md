# \_\_lsx_vssrani_d_q

LSX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/shift/index.md)

### Synopsis

```
__m128i __lsx_vssrani_d_q (__m128i a, __m128i b, imm0_127 imm)
#include <lsxintrin.h>
Instruction: vssrani.d.q vr, vr, imm
CPU Flags: LSX
```

### Description

Arithmetic right shift the signed 128-bit elements in `a` and `b` by `imm`, saturate the result to signed 64-bit, write the 64-bit results into `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  if (i < 1) {
    s128 temp = (s128)b.qword[i] >> imm;
    dst.dword[i] = clamp<s128>(temp, -9223372036854775808, 9223372036854775807);
  } else {
    s128 temp = (s128)a.qword[i - 1] >> imm;
    dst.dword[i] = clamp<s128>(temp, -9223372036854775808, 9223372036854775807);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 3       | 2                |
| 3A6000   | LA664  | 3       | 2                |
| 3C6000   | LA664  | 3       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
