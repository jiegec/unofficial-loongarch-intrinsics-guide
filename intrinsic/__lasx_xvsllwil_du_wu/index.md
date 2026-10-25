# \_\_lasx_xvsllwil_du_wu

LASX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shift/index.md)

### Synopsis

```
__m256i __lasx_xvsllwil_du_wu (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvsllwil.du.wu xr, xr, imm
CPU Flags: LASX
```

### Description

For each 128-bit lane, zero-extend and shift the 32-bit elements in the lower part of that lane by `imm` to 64-bit result.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (u64)(u32)a.word[i] << imm;
}
for (int i = 2; i < 4; i++) {
  dst.dword[i] = (u64)(u32)a.word[i + 2] << imm;
}

// Expands to:

if (0) {
  dst.dword[0] = ((u64)((u32)a.word[0])) << imm;
  dst.dword[1] = ((u64)((u32)a.word[1])) << imm;
  dst.dword[2] = ((u64)((u32)a.word[4])) << imm;
  dst.dword[3] = ((u64)((u32)a.word[5])) << imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 1                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
