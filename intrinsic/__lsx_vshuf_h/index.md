# \_\_lsx_vshuf_h

LSX intrinsic · group: [Shuffling](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/shuffling/index.md)

### Synopsis

```
__m128i __lsx_vshuf_h (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vshuf.h vr, vr, vr
CPU Flags: LSX
```

### Description

For each 128-bit lane, shuffle 16-bit elements from `b` and `c` using the low 4 bits of the indices from the corresponding lane of `a`, and save the result to `dst`.

### Examples

```
__m128i __lsx_vshuf_h(__m128i{0x0001000200030004, 0x0005000a000b000c}, __m128i{0x1122334455667788, 0x99aabbccddeeff00}, __m128i{0xabcdef1314156678, 0x1234123443214321})
= 0x1415ef13abcd4321 0x432133441122ff00
```

### Operation

```
for (int i = 0; i < 8; i++) {
  if ((a.half[i] % 256) >= 64 && (UARCH_LA264 || UARCH_LA464)) {
    // Caveat: observed in LA264 and LA464
    dst.half[i] = 0;
  } else if ((a.half[i] % 16) < 8) {
    dst.half[i] = c.half[a.half[i] % 8];
  } else {
    dst.half[i] = b.half[a.half[i] % 8];
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 2                |
| 3C6000   | LA664  | 1       | 2                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
