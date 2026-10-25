# \_\_lsx_vexth_qu_du

LSX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/misc/index.md)

### Synopsis

```
__m128i __lsx_vexth_qu_du (__m128i a)
#include <lsxintrin.h>
Instruction: vexth.qu.du vr, vr
CPU Flags: LSX
```

### Description

Zero-extend 64-bit elements in the higher half of every 128-bit vector of `a` to 128-bit.

### Operation

```
for (int i = 0; i < 1; i++) {
  dst.qword[i] = (u128)(u64)a.dword[i + 1];
}

// Expands to:

if (0) {
  dst.qword[0] = (u128)((u64)a.dword[1]);
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
