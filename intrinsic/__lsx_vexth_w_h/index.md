# \_\_lsx_vexth_w_h

LSX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/misc/index.md)

### Synopsis

```
__m128i __lsx_vexth_w_h (__m128i a)
#include <lsxintrin.h>
Instruction: vexth.w.h vr, vr
CPU Flags: LSX
```

### Description

Sign-extend 16-bit elements in the higher half of every 128-bit vector of `a` to 32-bit.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (s32)(s16)a.half[i + 4];
}

// Expands to:

if (0) {
  dst.word[0] = (s32)((s16)a.half[4]);
  dst.word[1] = (s32)((s16)a.half[5]);
  dst.word[2] = (s32)((s16)a.half[6]);
  dst.word[3] = (s32)((s16)a.half[7]);
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
