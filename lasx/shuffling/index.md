# Shuffling

## \_\_m256i \_\_lasx_xvshuf_b (\_\_m256i a, \_\_m256i b, \_\_m256i c)

### Synopsis

```
__m256i __lasx_xvshuf_b (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvshuf.b xr, xr, xr, xr
CPU Flags: LASX
```

### Description

Shuffle bytes from `a` and `b` with indices from `c`.

Caveat: the indices are placed in `c`, while in other `vshuf` intrinsics, they are placed in `a`.

### Examples

```
__m256i __lasx_xvshuf_b(__m256i{0x1122334455667788, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, __m256i{0xabcdef1314156678, 0x1234123443214321, 0x1234123443214321, 0x5678567856785678}, __m256i{0x1f1f00001a0a1b0b, 0x1111120213031404, 0x0102030405060708, 0x1112131405060708})
= 0x99997878ee21dd43 0x7777661555144413 0x4321433412341278 0x1234121212341278
```

### Operation

```
for (int i = 0; i < 32; i++) {
  if ((c.byte[i] % 256) >= 64 && (UARCH_LA264 || UARCH_LA464)) {
    // Caveat: observed in LA264 and LA464
    dst.byte[i] = 0;
  } else if ((c.byte[i] % 32) < 16) {
    dst.byte[i] = b.byte[(c.byte[i] % 32) + ((i >= 16) ? 16 : 0)];
  } else {
    dst.byte[i] = a.byte[(c.byte[i] % 32) + ((i >= 16) ? 0 : -16)];
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 2                |
| 3C6000 | LA664 | 1       | 2                |

## \_\_m256i \_\_lasx_xvshuf_h (\_\_m256i a, \_\_m256i b, \_\_m256i c)

### Synopsis

```
__m256i __lasx_xvshuf_h (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvshuf.h xr, xr, xr
CPU Flags: LASX
```

### Description

Shuffle 16-bit elements in `b` and `c` with indices from `a`, save the result to `dst`.

### Examples

```
__m256i __lasx_xvshuf_h(__m256i{0x0001000200030004, 0x0005000a000b000c, 0x000f000e00010002, 0x0008000900020001}, __m256i{0x1122334455667788, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, __m256i{0xabcdef1314156678, 0x1234123443214321, 0x1234123443214321, 0x5678567856785678})
= 0x1415ef13abcd4321 0x432133441122ff00 0xaabbaabb43211234 0x1234123412344321
```

### Operation

```
for (int i = 0; i < 16; i++) {
  if ((a.half[i] % 256) >= 64 && (UARCH_LA264 || UARCH_LA464)) {
    // Caveat: observed in LA264 and LA464
    dst.half[i] = 0;
  } else if ((a.half[i] % 16) < 8) {
    dst.half[i] = c.half[(a.half[i] % 16) + ((i >= 8) ? 8 : 0)];
  } else {
    dst.half[i] = b.half[(a.half[i] % 16) + ((i >= 8) ? 0 : -8)];
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 2                |
| 3C6000 | LA664 | 1       | 2                |

## \_\_m256i \_\_lasx_xvshuf_w (\_\_m256i a, \_\_m256i b, \_\_m256i c)

### Synopsis

```
__m256i __lasx_xvshuf_w (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvshuf.w xr, xr, xr
CPU Flags: LASX
```

### Description

Shuffle 32-bit elements in `b` and `c` with indices from `a`, save the result to `dst`.

### Examples

```
__m256i __lasx_xvshuf_w(__m256i{0x0000000200000004, 0x0000000700000005, 0x0000000100000003, 0x0000000400000000}, __m256i{0x1122334455667788, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, __m256i{0xabcdef1314156678, 0x1234123443214321, 0x1234123443214321, 0x5678567856785678})
= 0x4321432155667788 0x99aabbcc11223344 0x1234123456785678 0x1234123443214321
```

### Operation

```
for (int i = 0; i < 8; i++) {
  if ((a.word[i] % 256) >= 64 && (UARCH_LA264 || UARCH_LA464)) {
    // Caveat: observed in LA264 and LA464
    dst.word[i] = 0;
  } else if ((a.word[i] % 8) < 4) {
    dst.word[i] = c.word[(a.word[i] % 8) + ((i >= 4) ? 4 : 0)];
  } else {
    dst.word[i] = b.word[(a.word[i] % 8) + ((i >= 4) ? 0 : -4)];
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 2                |
| 3C6000 | LA664 | 1       | 2                |

## \_\_m256i \_\_lasx_xvshuf_d (\_\_m256i a, \_\_m256i b, \_\_m256i c)

### Synopsis

```
__m256i __lasx_xvshuf_d (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvshuf.d xr, xr, xr
CPU Flags: LASX
```

### Description

Shuffle 64-bit elements in `b` and `c` with indices from `a`, save the result to `dst`.

### Examples

```
__m256i __lasx_xvshuf_d(__m256i{0x0000000000000000, 0x0000000000000003, 0x0000000000000002, 0x0000000000000001}, __m256i{0x1122334455667788, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, __m256i{0xabcdef1314156678, 0x1234123443214321, 0x1234123443214321, 0x5678567856785678})
= 0xabcdef1314156678 0x99aabbccddeeff00 0xabcdef1212341234 0x5678567856785678
```

### Operation

```
for (int i = 0; i < 4; i++) {
  if ((a.dword[i] % 256) >= 64 && (UARCH_LA264 || UARCH_LA464)) {
    // Caveat: observed in LA264 and LA464
    dst.dword[i] = 0;
  } else if ((a.dword[i] % 4) < 2) {
    dst.dword[i] = c.dword[(a.dword[i] % 4) + ((i >= 2) ? 2 : 0)];
  } else {
    dst.dword[i] = b.dword[(a.dword[i] % 4) + ((i >= 2) ? 0 : -2)];
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 2                |
| 3C6000 | LA664 | 1       | 2                |

## \_\_m256i \_\_lasx_xvshuf4i_b (\_\_m256i a, imm0_255 imm)

### Synopsis

```
__m256i __lasx_xvshuf4i_b (__m256i a, imm0_255 imm)
#include <lasxintrin.h>
Instruction: xvshuf4i.b xr, xr, imm
CPU Flags: LASX
```

### Description

Shuffle every four 8-bit elements in `a` with indices packed in `imm`, save the result to `dst`.

### Examples

```
__m256i __lasx_xvshuf4i_b( __m256i{ 0xabcdef1314156678, 0x1234123443214321, 0x1234123443214321, 0x5678567856785678}, 0x12)
= 0x13ef13cd78667815 0x3412343421432121 0x3412343421432121 0x7856787878567878
```

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = a.byte[(i & ~0x3) + ((imm >> (2 * (i & 0x3))) & 0x3)];
}

// Expands to:

if (0) {
  dst.byte[0] = a.byte[0 + ((imm >> 0) & 0x3)];
  dst.byte[1] = a.byte[0 + ((imm >> 2) & 0x3)];
  dst.byte[2] = a.byte[0 + ((imm >> 4) & 0x3)];
  dst.byte[3] = a.byte[0 + ((imm >> 6) & 0x3)];
  dst.byte[4] = a.byte[4 + ((imm >> 0) & 0x3)];
  dst.byte[5] = a.byte[4 + ((imm >> 2) & 0x3)];
  dst.byte[6] = a.byte[4 + ((imm >> 4) & 0x3)];
  dst.byte[7] = a.byte[4 + ((imm >> 6) & 0x3)];
  dst.byte[8] = a.byte[8 + ((imm >> 0) & 0x3)];
  dst.byte[9] = a.byte[8 + ((imm >> 2) & 0x3)];
  dst.byte[10] = a.byte[8 + ((imm >> 4) & 0x3)];
  dst.byte[11] = a.byte[8 + ((imm >> 6) & 0x3)];
  dst.byte[12] = a.byte[12 + ((imm >> 0) & 0x3)];
  dst.byte[13] = a.byte[12 + ((imm >> 2) & 0x3)];
  dst.byte[14] = a.byte[12 + ((imm >> 4) & 0x3)];
  dst.byte[15] = a.byte[12 + ((imm >> 6) & 0x3)];
  dst.byte[16] = a.byte[16 + ((imm >> 0) & 0x3)];
  dst.byte[17] = a.byte[16 + ((imm >> 2) & 0x3)];
  dst.byte[18] = a.byte[16 + ((imm >> 4) & 0x3)];
  dst.byte[19] = a.byte[16 + ((imm >> 6) & 0x3)];
  dst.byte[20] = a.byte[20 + ((imm >> 0) & 0x3)];
  dst.byte[21] = a.byte[20 + ((imm >> 2) & 0x3)];
  dst.byte[22] = a.byte[20 + ((imm >> 4) & 0x3)];
  dst.byte[23] = a.byte[20 + ((imm >> 6) & 0x3)];
  dst.byte[24] = a.byte[24 + ((imm >> 0) & 0x3)];
  dst.byte[25] = a.byte[24 + ((imm >> 2) & 0x3)];
  dst.byte[26] = a.byte[24 + ((imm >> 4) & 0x3)];
  dst.byte[27] = a.byte[24 + ((imm >> 6) & 0x3)];
  dst.byte[28] = a.byte[28 + ((imm >> 0) & 0x3)];
  dst.byte[29] = a.byte[28 + ((imm >> 2) & 0x3)];
  dst.byte[30] = a.byte[28 + ((imm >> 4) & 0x3)];
  dst.byte[31] = a.byte[28 + ((imm >> 6) & 0x3)];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvshuf4i_h (\_\_m256i a, imm0_255 imm)

### Synopsis

```
__m256i __lasx_xvshuf4i_h (__m256i a, imm0_255 imm)
#include <lasxintrin.h>
Instruction: xvshuf4i.h xr, xr, imm
CPU Flags: LASX
```

### Description

Shuffle every four 16-bit elements in `a` with indices packed in `imm`, save the result to `dst`.

### Examples

```
__m256i __lasx_xvshuf4i_h( __m256i{ 0xabcdef1314156678, 0x1234123443214321, 0x1234123443214321, 0x5678567856785678}, 0x12)
= 0x667814156678ef13 0x4321432143211234 0x4321432143211234 0x5678567856785678
```

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = a.half[(i & ~0x3) + ((imm >> (2 * (i & 0x3))) & 0x3)];
}

// Expands to:

if (0) {
  dst.half[0] = a.half[0 + ((imm >> 0) & 0x3)];
  dst.half[1] = a.half[0 + ((imm >> 2) & 0x3)];
  dst.half[2] = a.half[0 + ((imm >> 4) & 0x3)];
  dst.half[3] = a.half[0 + ((imm >> 6) & 0x3)];
  dst.half[4] = a.half[4 + ((imm >> 0) & 0x3)];
  dst.half[5] = a.half[4 + ((imm >> 2) & 0x3)];
  dst.half[6] = a.half[4 + ((imm >> 4) & 0x3)];
  dst.half[7] = a.half[4 + ((imm >> 6) & 0x3)];
  dst.half[8] = a.half[8 + ((imm >> 0) & 0x3)];
  dst.half[9] = a.half[8 + ((imm >> 2) & 0x3)];
  dst.half[10] = a.half[8 + ((imm >> 4) & 0x3)];
  dst.half[11] = a.half[8 + ((imm >> 6) & 0x3)];
  dst.half[12] = a.half[12 + ((imm >> 0) & 0x3)];
  dst.half[13] = a.half[12 + ((imm >> 2) & 0x3)];
  dst.half[14] = a.half[12 + ((imm >> 4) & 0x3)];
  dst.half[15] = a.half[12 + ((imm >> 6) & 0x3)];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvshuf4i_w (\_\_m256i a, imm0_255 imm)

### Synopsis

```
__m256i __lasx_xvshuf4i_w (__m256i a, imm0_255 imm)
#include <lasxintrin.h>
Instruction: xvshuf4i.w xr, xr, imm
CPU Flags: LASX
```

### Description

Shuffle every four 32-bit elements in `a` with indices packed in `imm`, save the result to `dst`.

### Examples

```
__m256i __lasx_xvshuf4i_w( __m256i{ 0xabcdef1314156678, 0x1234123443214321, 0x1234123443214321, 0x5678567856785678}, 0x12)
= 0x1415667843214321 0x14156678abcdef13 0x4321432156785678 0x4321432112341234
```

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = a.word[(i & ~0x3) + ((imm >> (2 * (i & 0x3))) & 0x3)];
}

// Expands to:

if (0) {
  dst.word[0] = a.word[0 + ((imm >> 0) & 0x3)];
  dst.word[1] = a.word[0 + ((imm >> 2) & 0x3)];
  dst.word[2] = a.word[0 + ((imm >> 4) & 0x3)];
  dst.word[3] = a.word[0 + ((imm >> 6) & 0x3)];
  dst.word[4] = a.word[4 + ((imm >> 0) & 0x3)];
  dst.word[5] = a.word[4 + ((imm >> 2) & 0x3)];
  dst.word[6] = a.word[4 + ((imm >> 4) & 0x3)];
  dst.word[7] = a.word[4 + ((imm >> 6) & 0x3)];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvshuf4i_d (\_\_m256i a, \_\_m256i b, imm0_255 imm)

### Synopsis

```
__m256i __lasx_xvshuf4i_d (__m256i a, __m256i b, imm0_255 imm)
#include <lasxintrin.h>
Instruction: xvshuf4i.d xr, xr, imm
CPU Flags: LASX
```

### Description

Shuffle every four 64-bit elements in `a` and `b` with indices packed in `imm`, save the result to `dst`.

### Examples

```
__m256i __lasx_xvshuf4i_d( __m256i{ 0x1122334455667788, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, __m256i{ 0xabcdef1314156678, 0x1234123443214321, 0x1234123443214321, 0x5678567856785678}, 0x12)
= 0xabcdef1314156678 0x1122334455667788 0x1234123443214321 0xabcdef1212341234
```

### Operation

```
dst.dword[0] = (imm & 2) ? b.dword[(imm & 1)] : a.dword[(imm & 1)];
dst.dword[1] =
    (imm & 8) ? b.dword[((imm >> 2) & 1)] : a.dword[((imm >> 2) & 1)];
dst.dword[2] = (imm & 2) ? b.dword[(imm & 1) + 2] : a.dword[(imm & 1) + 2];
dst.dword[3] =
    (imm & 8) ? b.dword[((imm >> 2) & 1) + 2] : a.dword[((imm >> 2) & 1) + 2];
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |
