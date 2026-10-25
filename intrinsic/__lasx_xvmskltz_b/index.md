# \_\_lasx_xvmskltz_b

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

### Synopsis

```
__m256i __lasx_xvmskltz_b (__m256i a)
#include <lasxintrin.h>
Instruction: xvmskltz.b xr, xr
CPU Flags: LASX
```

### Description

For each 128-bit lane, for each 8-bit element in `a`, set the bit at that element's index within the lane to one if the element is less than zero, otherwise zero; the remaining bits of the lane are zero.

### Examples

```
__m256i __lasx_xvmskltz_b(__m256i{0x1122334455667788, 0x99aabbccddeeff00, 0xabababab12121212, 0x1234567812345678})
= 0x000000000000fe01 0x0000000000000000 0x00000000000000f0 0x0000000000000000
__m256i __lasx_xvmskltz_b(__m256i{0x0000118100000000, 0x0081000081111118, 0x1181000001010801, 0x0000000000000000})
= 0x0000000000004810 0x0000000000000000 0x0000000000000040 0x0000000000000000
```

### Operation

```
u64 m = 0x8080808080808080;
u64 c = m & a.dword[0];
c |= c << 7;
c |= c << 14;
c |= c << 28;
c >>= 56;
dst.dword[0] = c;
c = m & a.dword[1];
c |= c << 7;
c |= c << 14;
c |= c << 28;
c >>= 56;
dst.dword[0] |= c << 8;
dst.dword[1] = 0;

c = m & a.dword[2];
c |= c << 7;
c |= c << 14;
c |= c << 28;
c >>= 56;
dst.dword[2] = c;
c = m & a.dword[3];
c |= c << 7;
c |= c << 14;
c |= c << 28;
c >>= 56;
dst.dword[2] |= c << 8;
dst.dword[3] = 0;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
