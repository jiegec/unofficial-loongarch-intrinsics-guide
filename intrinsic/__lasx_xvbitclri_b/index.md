# \_\_lasx_xvbitclri_b

LASX intrinsic · group: [Bitwise Operations](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/bitwise_operations/index.md)

### Synopsis

```
__m256i __lasx_xvbitclri_b (__m256i a, imm0_7 imm)
#include <lasxintrin.h>
Instruction: xvbitclri.b xr, xr, imm
CPU Flags: LASX
```

### Description

For each 8-bit element, clear the bit indexed by `imm`, save the result in `dst`.

### Examples

```
__m256i __lasx_xvbitclri_b( __m256i{ 0xffffffffffffffff, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, 1)
= 0xfdfdfdfdfdfdfdfd 0x99a8b9ccddecfd00 0xa9cded1010341034 0xa8b9a8b9ddecddec
```

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = a.byte[i] & (~((u8)1 << imm));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
