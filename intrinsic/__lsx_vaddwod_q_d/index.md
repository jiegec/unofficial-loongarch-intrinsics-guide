# \_\_lsx_vaddwod_q_d

LSX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_computation/index.md)

### Synopsis

```
__m128i __lsx_vaddwod_q_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vaddwod.q.d vr, vr, vr
CPU Flags: LSX
```

### Description

Add the odd-positioned signed 64-bit elements in `a` and the corresponding signed 64-bit elements in `b`, extending both to 128-bit, and save the 128-bit result in `dst`.

### Operation

```
for (int i = 0; i < 1; i++) {
  dst.qword[i] = (s128)(s64)a.dword[2 * i + 1] + (s128)(s64)b.dword[2 * i + 1];
}

// Expands to:

if (0) {
  dst.qword[0] = ((s128)((s64)a.dword[1])) + ((s128)((s64)b.dword[1]));
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
