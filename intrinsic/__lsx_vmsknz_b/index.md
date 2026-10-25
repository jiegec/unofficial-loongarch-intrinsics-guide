# \_\_lsx_vmsknz_b

LSX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/misc/index.md)

### Synopsis

```
__m128i __lsx_vmsknz_b (__m128i a)
#include <lsxintrin.h>
Instruction: vmsknz.b vr, vr
CPU Flags: LSX
```

### Description

For each 128-bit lane, for each 8-bit element in `a`, set the bit at that element's index within the lane to one if the element is non-zero, otherwise zero; the remaining bits of the lane are zero.

### Examples

```
__m128i __lsx_vmsknz_b(__m128i{0x1122334455667788, 0x99aabbccddeeff00})
= 0x000000000000feff 0x0000000000000000
__m128i __lsx_vmsknz_b(__m128i{0x0000111100000000, 0x0011000011111111})
= 0x0000000000004f30 0x0000000000000000
```

### Operation

```
u64 m = 0x7F7F7F7F7F7F7F7F;
u64 c = ~(((a.dword[0] & m) + m) | a.dword[0] | m);
c |= c << 7;
c |= c << 14;
c |= c << 28;
c >>= 56;
dst.dword[0] = c;
c = ~(((a.dword[1] & m) + m) | a.dword[1] | m);
c |= c << 7;
c |= c << 14;
c |= c << 28;
c >>= 56;
dst.dword[0] |= c << 8;
dst.dword[0] = (u16)~dst.dword[0];
dst.dword[1] = 0;
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
