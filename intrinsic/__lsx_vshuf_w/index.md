# \_\_lsx_vshuf_w

LSX intrinsic · group: [Shuffling](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/shuffling/index.md)

### Synopsis

```
__m128i __lsx_vshuf_w (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vshuf.w vr, vr, vr
CPU Flags: LSX
```

### Description

For each 128-bit lane, shuffle 32-bit elements from `b` and `c` using the low 3 bits of the indices from the corresponding lane of `a`, and save the result to `dst`.

### Examples

```
__m128i __lsx_vshuf_w(__m128i{0x0000000200000004, 0x0000000700000005}, __m128i{0x1122334455667788, 0x99aabbccddeeff00}, __m128i{0xabcdef1314156678, 0x1234123443214321})
= 0x4321432155667788 0x99aabbcc11223344
```

### Operation

```
for (int i = 0; i < 4; i++) {
  if ((a.word[i] % 256) >= 64 && (UARCH_LA264 || UARCH_LA464)) {
    // Caveat: observed in LA264 and LA464
    dst.word[i] = 0;
  } else if ((a.word[i] % 8) < 4) {
    dst.word[i] = c.word[a.word[i] % 4];
  } else {
    dst.word[i] = b.word[a.word[i] % 4];
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
