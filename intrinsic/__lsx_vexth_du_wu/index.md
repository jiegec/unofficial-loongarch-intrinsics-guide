# \_\_lsx_vexth_du_wu

LSX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/misc/index.md)

### Synopsis

```
__m128i __lsx_vexth_du_wu (__m128i a)
#include <lsxintrin.h>
Instruction: vexth.du.wu vr, vr
CPU Flags: LSX
```

### Description

Zero-extend 32-bit elements in the higher half of every 128-bit vector of `a` to 64-bit.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (u64)(u32)a.word[i + 2];
}

// Expands to:

if (0) {
  dst.dword[0] = (u64)((u32)a.word[2]);
  dst.dword[1] = (u64)((u32)a.word[3]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
