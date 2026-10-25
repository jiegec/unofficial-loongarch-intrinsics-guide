# \_\_lsx_vbitclr_d

LSX intrinsic · group: [Bitwise Operations](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/bitwise_operations/index.md)

### Synopsis

```
__m128i __lsx_vbitclr_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vbitclr.d vr, vr, vr
CPU Flags: LSX
```

### Description

For each 64-bit element, clear the bit indexed by the corresponding element of `b` modulo 64, save the result in `dst`.

### Examples

```
__m128i __lsx_vbitclr_d(__m128i{0xffffffffffffffff, 0x99aabbccddeeff00}, __m128i{0xabababababababab, 0x1234123443214321})
= 0xfffff7ffffffffff 0x99aabbccddeeff00
```

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = a.dword[i] & (~((u64)1 << (b.dword[i] % 64)));
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
