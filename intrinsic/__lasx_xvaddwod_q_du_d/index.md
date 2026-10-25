# \_\_lasx_xvaddwod_q_du_d

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvaddwod_q_du_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvaddwod.q.du.d xr, xr, xr
CPU Flags: LASX
```

### Description

Add the odd-positioned unsigned 64-bit elements in `a` and the corresponding signed 64-bit elements in `b`, extending both to 128-bit, and save the 128-bit result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.qword[i] = (u128)(u64)a.dword[2 * i + 1] + (s128)(s64)b.dword[2 * i + 1];
}

// Expands to:

if (0) {
  dst.qword[0] = ((u128)((u64)a.dword[1])) + ((s128)((s64)b.dword[1]));
  dst.qword[1] = ((u128)((u64)a.dword[3])) + ((s128)((s64)b.dword[3]));
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
