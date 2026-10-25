# \_\_lsx_vhaddw_hu_bu

LSX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_computation/index.md)

### Synopsis

```
__m128i __lsx_vhaddw_hu_bu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vhaddw.hu.bu vr, vr, vr
CPU Flags: LSX
```

### Description

Add odd-positioned unsigned 8-bit elements in `a` to even-positioned unsigned 8-bit elements in `b`, zero-extending both to 16-bit, to get the 16-bit result.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (u16)(u8)a.byte[2 * i + 1] + (u16)(u8)b.byte[2 * i];
}

// Expands to:

if (0) {
  dst.half[0] = ((u16)((u8)a.byte[1])) + ((u16)((u8)b.byte[0]));
  dst.half[1] = ((u16)((u8)a.byte[3])) + ((u16)((u8)b.byte[2]));
  dst.half[2] = ((u16)((u8)a.byte[5])) + ((u16)((u8)b.byte[4]));
  dst.half[3] = ((u16)((u8)a.byte[7])) + ((u16)((u8)b.byte[6]));
  dst.half[4] = ((u16)((u8)a.byte[9])) + ((u16)((u8)b.byte[8]));
  dst.half[5] = ((u16)((u8)a.byte[11])) + ((u16)((u8)b.byte[10]));
  dst.half[6] = ((u16)((u8)a.byte[13])) + ((u16)((u8)b.byte[12]));
  dst.half[7] = ((u16)((u8)a.byte[15])) + ((u16)((u8)b.byte[14]));
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
