# \_\_lsx_vshuf_b

LSX intrinsic · group: [Shuffling](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/shuffling/index.md)

### Synopsis

```
__m128i __lsx_vshuf_b (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vshuf.b vr, vr, vr, vr
CPU Flags: LSX
```

### Description

For each 128-bit lane, shuffle bytes from `a` and `b` using the low 5 bits of the indices from the corresponding lane of `c`.

Caveat: the indices are placed in `c`, while in other `vshuf` intrinsics, they are placed in `a`.

Caution: the result is undefined when the highest 3 bits of the index byte are non-zero.

### Examples

```
__m128i __lsx_vshuf_b(__m128i{0x1122334455667788, 0x99aabbccddeeff00}, __m128i{0xabcdef1314156678, 0x1234123443214321}, __m128i{0x0011021304050607, 0x0811120213031404})
= 0x7877155513efcdab 0x2177661555144413
```

### Operation

```
for (int i = 0; i < 16; i++) {
  if (c.byte[i] >= 64 && (UARCH_LA264 || UARCH_LA464)) {
    // Caveat: observed in LA264 and LA464
    dst.byte[i] = 0;
  } else if ((c.byte[i] % 32) < 16) {
    dst.byte[i] = b.byte[c.byte[i] % 16];
  } else {
    dst.byte[i] = a.byte[c.byte[i] % 16];
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
