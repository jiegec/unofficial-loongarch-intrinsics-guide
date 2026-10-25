# \_\_lsx_vbitrev_w

LSX intrinsic · group: [Bitwise Operations](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/bitwise_operations/index.md)

### Synopsis

```
__m128i __lsx_vbitrev_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vbitrev.w vr, vr, vr
CPU Flags: LSX
```

### Description

For each 32-bit element, toggle the bit indexed by the corresponding element of `b` modulo 32, save the result in `dst`.

### Examples

```
__m128i __lsx_vbitrev_w(__m128i{0x0f0f0f0f0f0f0f0f, 0x99aabbccddeeff00}, __m128i{0xabababababababab, 0x1234123443214321})
= 0x0f0f070f0f0f070f 0x99babbccddeeff02
```

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = a.word[i] ^ ((u32)1 << (b.word[i] % 32));
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
