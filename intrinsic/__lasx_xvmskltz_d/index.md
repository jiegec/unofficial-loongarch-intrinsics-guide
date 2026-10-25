# \_\_lasx_xvmskltz_d

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

### Synopsis

```
__m256i __lasx_xvmskltz_d (__m256i a)
#include <lasxintrin.h>
Instruction: xvmskltz.d xr, xr
CPU Flags: LASX
```

### Description

For each 128-bit lane, for each 64-bit element in `a`, set the bit at that element's index within the lane to one if the element is less than zero, otherwise zero; the remaining bits of the lane are zero.

### Examples

```
__m256i __lasx_xvmskltz_d(__m256i{0x1122334455667788, 0x99aabbccddeeff00, 0xabababab12121212, 0x1234567812345678})
= 0x0000000000000002 0x0000000000000000 0x0000000000000001 0x0000000000000000
__m256i __lasx_xvmskltz_d(__m256i{0x0000111800000000, 0x0081000081111111, 0x8111000008010101, 0x0000000000000000})
= 0x0000000000000000 0x0000000000000000 0x0000000000000001 0x0000000000000000
```

### Operation

```
u64 m = 0x8000000000000000;
u64 c = m & a.dword[0];
c >>= 63;
dst.dword[0] = c;
c = m & a.dword[1];
c >>= 63;
dst.dword[0] |= c << 1;
dst.dword[1] = 0;

c = m & a.dword[2];
c >>= 63;
dst.dword[2] = c;
c = m & a.dword[3];
c >>= 63;
dst.dword[2] |= c << 1;
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
