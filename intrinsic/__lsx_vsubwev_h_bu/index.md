# \_\_lsx_vsubwev_h_bu

LSX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_computation/index.md)

### Synopsis

```
__m128i __lsx_vsubwev_h_bu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsubwev.h.bu vr, vr, vr
CPU Flags: LSX
```

### Description

Subtract the even-positioned unsigned 8-bit elements in `b` from the corresponding elements in `a`, extending both to 16-bit, and save the 16-bit result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (u16)(u8)a.byte[2 * i] - (u16)(u8)b.byte[2 * i];
}

// Expands to:

if (0) {
  dst.half[0] = ((u16)((u8)a.byte[0])) - ((u16)((u8)b.byte[0]));
  dst.half[1] = ((u16)((u8)a.byte[2])) - ((u16)((u8)b.byte[2]));
  dst.half[2] = ((u16)((u8)a.byte[4])) - ((u16)((u8)b.byte[4]));
  dst.half[3] = ((u16)((u8)a.byte[6])) - ((u16)((u8)b.byte[6]));
  dst.half[4] = ((u16)((u8)a.byte[8])) - ((u16)((u8)b.byte[8]));
  dst.half[5] = ((u16)((u8)a.byte[10])) - ((u16)((u8)b.byte[10]));
  dst.half[6] = ((u16)((u8)a.byte[12])) - ((u16)((u8)b.byte[12]));
  dst.half[7] = ((u16)((u8)a.byte[14])) - ((u16)((u8)b.byte[14]));
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
