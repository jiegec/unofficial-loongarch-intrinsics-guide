# \_\_lasx_xvmaddwod_q_du_d

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvmaddwod_q_du_d (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvmaddwod.q.du.d xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply the odd-positioned unsigned 64-bit elements in `b` and the corresponding signed 64-bit elements in `c`, extending both to 128-bit, and add the product to the 128-bit elements in `a`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.qword[i] = (u128)(u64)b.dword[2 * i + 1] * (s128)(s64)c.dword[2 * i + 1] +
                 (s128)a.qword[i];
}

// Expands to:

if (0) {
  dst.qword[0] = (((u128)((u64)b.dword[1])) * ((s128)((s64)c.dword[1]))) +
                 ((s128)a.qword[0]);
  dst.qword[1] = (((u128)((u64)b.dword[3])) * ((s128)((s64)c.dword[3]))) +
                 ((s128)a.qword[1]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 7       | 1.14             |
| 3A6000 | LA664 | 7       | 1.14             |
| 3C6000 | LA664 | 7       | 1.14             |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
