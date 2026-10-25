# \_\_lasx_xvhaddw_qu_du

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvhaddw_qu_du (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvhaddw.qu.du xr, xr, xr
CPU Flags: LASX
```

### Description

Add odd-positioned unsigned 64-bit elements in `a` to even-positioned unsigned 64-bit elements in `b`, zero-extending both to 128-bit, to get the 128-bit result.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.qword[i] = (u128)(u64)a.dword[2 * i + 1] + (u128)(u64)b.dword[2 * i];
}

// Expands to:

if (0) {
  dst.qword[0] = ((u128)((u64)a.dword[1])) + ((u128)((u64)b.dword[0]));
  dst.qword[1] = ((u128)((u64)a.dword[3])) + ((u128)((u64)b.dword[2]));
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
