# \_\_lsx_vsubwev_w_hu

LSX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_computation/index.md)

### Synopsis

```
__m128i __lsx_vsubwev_w_hu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsubwev.w.hu vr, vr, vr
CPU Flags: LSX
```

### Description

Subtract the even-positioned unsigned 16-bit elements in `b` from the corresponding elements in `a`, extending both to 32-bit, and save the 32-bit result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (u32)(u16)a.half[2 * i] - (u32)(u16)b.half[2 * i];
}

// Expands to:

if (0) {
  dst.word[0] = ((u32)((u16)a.half[0])) - ((u32)((u16)b.half[0]));
  dst.word[1] = ((u32)((u16)a.half[2])) - ((u32)((u16)b.half[2]));
  dst.word[2] = ((u32)((u16)a.half[4])) - ((u32)((u16)b.half[4]));
  dst.word[3] = ((u32)((u16)a.half[6])) - ((u32)((u16)b.half[6]));
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
