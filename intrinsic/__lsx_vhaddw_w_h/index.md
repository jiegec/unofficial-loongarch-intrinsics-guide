# \_\_lsx_vhaddw_w_h

LSX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_computation/index.md)

### Synopsis

```
__m128i __lsx_vhaddw_w_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vhaddw.w.h vr, vr, vr
CPU Flags: LSX
```

### Description

Add odd-positioned signed 16-bit elements in `a` to even-positioned signed 16-bit elements in `b`, sign-extending both to 32-bit, to get the 32-bit result.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (s32)(s16)a.half[2 * i + 1] + (s32)(s16)b.half[2 * i];
}

// Expands to:

if (0) {
  dst.word[0] = ((s32)((s16)a.half[1])) + ((s32)((s16)b.half[0]));
  dst.word[1] = ((s32)((s16)a.half[3])) + ((s32)((s16)b.half[2]));
  dst.word[2] = ((s32)((s16)a.half[5])) + ((s32)((s16)b.half[4]));
  dst.word[3] = ((s32)((s16)a.half[7])) + ((s32)((s16)b.half[6]));
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
