# Shuffling

## \_\_m128i \_\_lsx_vshuf_b (\_\_m128i a, \_\_m128i b, \_\_m128i c)

### Synopsis

```
__m128i __lsx_vshuf_b (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vshuf.b vr, vr, vr, vr
CPU Flags: LSX
```

### Description

Shuffle bytes from `a` and `b` with indices from `c`.

Caveat: the indices are placed in `c`, while in other `vshuf` intrinsics, they are placed in `a`.

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

## \_\_m128i \_\_lsx_vshuf_h (\_\_m128i a, \_\_m128i b, \_\_m128i c)

### Synopsis

```
__m128i __lsx_vshuf_h (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vshuf.h vr, vr, vr
CPU Flags: LSX
```

### Description

Shuffle 16-bit elements in `b` and `c` with indices from `a`, save the result to `dst`.

### Examples

```
__m128i __lsx_vshuf_h(__m128i{0x0001000200030004, 0x0005000a000b000c}, __m128i{0x1122334455667788, 0x99aabbccddeeff00}, __m128i{0xabcdef1314156678, 0x1234123443214321})
= 0x1415ef13abcd4321 0x432133441122ff00
```

### Operation

```
for (int i = 0; i < 8; i++) {
  if ((a.half[i] % 256) >= 64 && (UARCH_LA264 || UARCH_LA464)) {
    // Caveat: observed in LA264 and LA464
    dst.half[i] = 0;
  } else if ((a.half[i] % 16) < 8) {
    dst.half[i] = c.half[a.half[i] % 8];
  } else {
    dst.half[i] = b.half[a.half[i] % 8];
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

## \_\_m128i \_\_lsx_vshuf_w (\_\_m128i a, \_\_m128i b, \_\_m128i c)

### Synopsis

```
__m128i __lsx_vshuf_w (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vshuf.w vr, vr, vr
CPU Flags: LSX
```

### Description

Shuffle 32-bit elements in `b` and `c` with indices from `a`, save the result to `dst`.

### Examples

```
__m128i __lsx_vshuf_w(__m128i{0x0000000200000004, 0x0000000700000005}, __m128i{0x1122334455667788, 0x99aabbccddeeff00}, __m128i{0xabcdef1314156678, 0x1234123443214321})
= 0x4321432155667788 0x99aabbcc11223344
```

### Operation

```
for (int i = 0; i < 4; i++) {
  if ((a.word[i] % 256) >= 64 && (UARCH_LA264 || UARCH_LA464)) {
    // Caveat: observed in LA264 and LA464
    dst.word[i] = 0;
  } else if ((a.word[i] % 8) < 4) {
    dst.word[i] = c.word[a.word[i] % 4];
  } else {
    dst.word[i] = b.word[a.word[i] % 4];
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

## \_\_m128i \_\_lsx_vshuf_d (\_\_m128i a, \_\_m128i b, \_\_m128i c)

### Synopsis

```
__m128i __lsx_vshuf_d (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vshuf.d vr, vr, vr
CPU Flags: LSX
```

### Description

Shuffle 64-bit elements in `b` and `c` with indices from `a`, save the result to `dst`.

### Examples

```
__m128i __lsx_vshuf_d(__m128i{0x0000000000000001, 0x0000000000000002}, __m128i{0x1122334455667788, 0x99aabbccddeeff00}, __m128i{0xabcdef1314156678, 0x1234123443214321})
= 0x1234123443214321 0x1122334455667788
```

### Operation

```
for (int i = 0; i < 2; i++) {
  if ((a.dword[i] % 256) >= 64 && (UARCH_LA264 || UARCH_LA464)) {
    // Caveat: observed in LA264 and LA464
    dst.dword[i] = 0;
  } else if ((a.dword[i] % 4) < 2) {
    dst.dword[i] = c.dword[a.dword[i] % 2];
  } else {
    dst.dword[i] = b.dword[a.dword[i] % 2];
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

## \_\_m128i \_\_lsx_vshuf4i_b (\_\_m128i a, imm0_255 imm)

### Synopsis

```
__m128i __lsx_vshuf4i_b (__m128i a, imm0_255 imm)
#include <lsxintrin.h>
Instruction: vshuf4i.b vr, vr, imm
CPU Flags: LSX
```

### Description

Shuffle every four 8-bit elements in `a` with indices packed in `imm`, save the result to `dst`.

### Examples

```
__m128i __lsx_vshuf4i_b(__m128i{0xabcdef1314156678, 0x1234123443214321}, 0x12)
= 0x13ef13cd78667815 0x3412343421432121
```

### Operation

```
for (int i = 0; i < 16; i++) {
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
}
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

## \_\_m128i \_\_lsx_vshuf4i_h (\_\_m128i a, imm0_255 imm)

### Synopsis

```
__m128i __lsx_vshuf4i_h (__m128i a, imm0_255 imm)
#include <lsxintrin.h>
Instruction: vshuf4i.h vr, vr, imm
CPU Flags: LSX
```

### Description

Shuffle every four 16-bit elements in `a` with indices packed in `imm`, save the result to `dst`.

### Examples

```
__m128i __lsx_vshuf4i_h(__m128i{0xabcdef1314156678, 0x1234123443214321}, 0x12)
= 0x667814156678ef13 0x4321432143211234
```

### Operation

```
for (int i = 0; i < 8; i++) {
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
}
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

## \_\_m128i \_\_lsx_vshuf4i_w (\_\_m128i a, imm0_255 imm)

### Synopsis

```
__m128i __lsx_vshuf4i_w (__m128i a, imm0_255 imm)
#include <lsxintrin.h>
Instruction: vshuf4i.w vr, vr, imm
CPU Flags: LSX
```

### Description

Shuffle every four 32-bit elements in `a` with indices packed in `imm`, save the result to `dst`.

### Examples

```
__m128i __lsx_vshuf4i_w(__m128i{0xabcdef1314156678, 0x1234123443214321}, 0x12)
= 0x1415667843214321 0x14156678abcdef13
```

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = a.word[(i & ~0x3) + ((imm >> (2 * (i & 0x3))) & 0x3)];
}

// Expands to:

if (0) {
  dst.word[0] = a.word[0 + ((imm >> 0) & 0x3)];
  dst.word[1] = a.word[0 + ((imm >> 2) & 0x3)];
  dst.word[2] = a.word[0 + ((imm >> 4) & 0x3)];
  dst.word[3] = a.word[0 + ((imm >> 6) & 0x3)];
}
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

## \_\_m128i \_\_lsx_vshuf4i_d (\_\_m128i a, \_\_m128i b, imm0_255 imm)

### Synopsis

```
__m128i __lsx_vshuf4i_d (__m128i a, __m128i b, imm0_255 imm)
#include <lsxintrin.h>
Instruction: vshuf4i.d vr, vr, imm
CPU Flags: LSX
```

### Description

Shuffle every four 64-bit elements in `a` and `b` with indices packed in `imm`, save the result to `dst`.

### Examples

```
__m128i __lsx_vshuf4i_d(__m128i{0x1122334455667788, 0x99aabbccddeeff00}, __m128i{0xabcdef1314156678, 0x1234123443214321}, 0x12)
= 0xabcdef1314156678 0x1122334455667788
```

### Operation

```
dst.dword[0] = (imm & 2) ? b.dword[(imm & 1)] : a.dword[(imm & 1)];
dst.dword[1] =
    (imm & 8) ? b.dword[((imm >> 2) & 1)] : a.dword[((imm >> 2) & 1)];
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
