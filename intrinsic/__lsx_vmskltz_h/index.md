# \_\_lsx_vmskltz_h

LSX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/misc/index.md)

### Synopsis

```
__m128i __lsx_vmskltz_h (__m128i a)
#include <lsxintrin.h>
Instruction: vmskltz.h vr, vr
CPU Flags: LSX
```

### Description

For each 128-bit lane, for each 16-bit element in `a`, set the bit at that element's index within the lane to one if the element is less than zero, otherwise zero; the remaining bits of the lane are zero.

### Examples

```
__m128i __lsx_vmskltz_h(__m128i{0x1122334455667788, 0x99aabbccddeeff00})
= 0x00000000000000f0 0x0000000000000000
__m128i __lsx_vmskltz_h(__m128i{0x0000808000000000, 0x0081000081716151})
= 0x0000000000000024 0x0000000000000000
```

### Operation

```
u64 m = 0x8000800080008000;
u64 c = m & a.dword[0];
c |= c << 15;
c |= c << 30;
c >>= 60;
dst.dword[0] = c;
c = m & a.dword[1];
c |= c << 15;
c |= c << 30;
c >>= 60;
dst.dword[0] |= c << 4;
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
