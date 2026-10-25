# \_\_lsx_vhaddw_du_wu

LSX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_computation/index.md)

### Synopsis

```
__m128i __lsx_vhaddw_du_wu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vhaddw.du.wu vr, vr, vr
CPU Flags: LSX
```

### Description

Add odd-positioned unsigned 32-bit elements in `a` to even-positioned unsigned 32-bit elements in `b`, zero-extending both to 64-bit, to get the 64-bit result.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (u64)(u32)a.word[2 * i + 1] + (u64)(u32)b.word[2 * i];
}

// Expands to:

if (0) {
  dst.dword[0] = ((u64)((u32)a.word[1])) + ((u64)((u32)b.word[0]));
  dst.dword[1] = ((u64)((u32)a.word[3])) + ((u64)((u32)b.word[2]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
