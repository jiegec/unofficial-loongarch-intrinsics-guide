# Permutation

## \_\_m256i \_\_lasx_xvpermi_w (\_\_m256i a, \_\_m256i b, imm0_255 imm)

### Synopsis

```
__m256i __lasx_xvpermi_w (__m256i a, __m256i b, imm0_255 imm)
#include <lasxintrin.h>
Instruction: xvpermi.w xr, xr, imm
CPU Flags: LASX
```

### Description

Permute words from `a` and `b` with indices recorded in `imm` and store into `dst`.

### Examples

```
__m256i __lasx_xvpermi_w( __m256i{ 0x1122334455667788, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, __m256i{ 0xababababbbbbbbbb, 0x1234123443214321, 0x1234123443214321, 0x5678567856785678}, 0x12)
= 0xbbbbbbbb43214321 0x5566778811223344 0x4321432156785678 0x12341234abcdef12
```

### Operation

```
dst.word[0] = b.word[imm & 0x3];
dst.word[1] = b.word[(imm >> 2) & 0x3];
dst.word[2] = a.word[(imm >> 4) & 0x3];
dst.word[3] = a.word[(imm >> 6) & 0x3];
dst.word[4] = b.word[4 + (imm & 0x3)];
dst.word[5] = b.word[4 + ((imm >> 2) & 0x3)];
dst.word[6] = a.word[4 + ((imm >> 4) & 0x3)];
dst.word[7] = a.word[4 + ((imm >> 6) & 0x3)];
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvpermi_d (\_\_m256i a, imm0_255 imm)

### Synopsis

```
__m256i __lasx_xvpermi_d (__m256i a, imm0_255 imm)
#include <lasxintrin.h>
Instruction: xvpermi.d xr, xr, imm
CPU Flags: LASX
```

### Description

Permute double words from `a` with indices recorded in `imm` and store into `dst`.

### Examples

```
__m256i __lasx_xvpermi_d( __m256i{ 0x1122334455667788, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, 0x12)
= 0xabcdef1212341234 0x1122334455667788 0x99aabbccddeeff00 0x1122334455667788
```

### Operation

```
dst.dword[0] = a.dword[imm & 0x3];
dst.dword[1] = a.dword[(imm >> 2) & 0x3];
dst.dword[2] = a.dword[(imm >> 4) & 0x3];
dst.dword[3] = a.dword[(imm >> 6) & 0x3];
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 4                |
| 3C6000 | LA664 | 3       | 4                |

## \_\_m256i \_\_lasx_xvpermi_q (\_\_m256i a, \_\_m256i b, imm0_255 imm)

### Synopsis

```
__m256i __lasx_xvpermi_q (__m256i a, __m256i b, imm0_255 imm)
#include <lasxintrin.h>
Instruction: xvpermi.q xr, xr, imm
CPU Flags: LASX
```

### Description

Permute quad words from `a` and `b` with indices recorded in `imm` and store into `dst`.

### Examples

```
__m256i __lasx_xvpermi_q( __m256i{ 0x1122334455667788, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, __m256i{ 0xababababbbbbbbbb, 0x1234123443214321, 0x1234123443214321, 0x5678567856785678}, 0x12)
= 0x1122334455667788 0x99aabbccddeeff00 0x1234123443214321 0x5678567856785678
```

### Operation

```
if ((imm & 0x4) && (UARCH_LA264 || UARCH_LA464)) {
  // Caveat: observed in LA264 and LA464
  dst.qword[0] = 0;
} else {
  dst.qword[0] = (imm & 2) ? a.qword[imm & 0x1] : b.qword[imm & 0x1];
}
if ((imm & 0x80) && (UARCH_LA264 || UARCH_LA464)) {
  // Caveat: observed in LA264 and LA464
  dst.qword[1] = 0;
} else {
  dst.qword[1] =
      (imm & 0x20) ? a.qword[(imm >> 4) & 0x1] : b.qword[(imm >> 4) & 0x1];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2.67             |
| 3C6000 | LA664 | 3       | 2.67             |

## \_\_m256i \_\_lasx_xvperm_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvperm_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvperm.w xr, xr, xr
CPU Flags: LASX
```

### Description

Permute words from `a` with indices recorded in `b` and store into `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = a.word[b.word[i] % 0x8];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 4                |
| 3C6000 | LA664 | 3       | 4                |
