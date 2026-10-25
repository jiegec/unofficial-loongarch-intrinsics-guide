# \_\_lsx_vexth_h_b

LSX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/misc/index.md)

### Synopsis

```
__m128i __lsx_vexth_h_b (__m128i a)
#include <lsxintrin.h>
Instruction: vexth.h.b vr, vr
CPU Flags: LSX
```

### Description

Sign-extend 8-bit elements in the higher half of every 128-bit vector of `a` to 16-bit.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (s16)(s8)a.byte[i + 8];
}

// Expands to:

if (0) {
  dst.half[0] = (s16)((s8)a.byte[8]);
  dst.half[1] = (s16)((s8)a.byte[9]);
  dst.half[2] = (s16)((s8)a.byte[10]);
  dst.half[3] = (s16)((s8)a.byte[11]);
  dst.half[4] = (s16)((s8)a.byte[12]);
  dst.half[5] = (s16)((s8)a.byte[13]);
  dst.half[6] = (s16)((s8)a.byte[14]);
  dst.half[7] = (s16)((s8)a.byte[15]);
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
