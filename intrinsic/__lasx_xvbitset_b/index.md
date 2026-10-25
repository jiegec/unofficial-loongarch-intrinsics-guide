# \_\_lasx_xvbitset_b

LASX intrinsic · group: [Bitwise Operations](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/bitwise_operations/index.md)

### Synopsis

```
__m256i __lasx_xvbitset_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvbitset.b xr, xr, xr
CPU Flags: LASX
```

### Description

For each 8-bit element, set the bit indexed by the corresponding element of `b` modulo 8, save the result in `dst`.

### Examples

```
__m256i __lasx_xvbitset_b(__m256i{0x0000000000000000, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, __m256i{0xabababababababab, 0x1234123443214321, 0x1234123443214321, 0x5678567856785678})
= 0x0808080808080808 0x9dbabfdcddeeff02 0xafddef121a361a36 0xeabbeabbddefddef
```

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = a.byte[i] | ((u8)1 << (b.byte[i] % 8));
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
