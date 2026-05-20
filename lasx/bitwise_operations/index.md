# Bitwise Operations

## \_\_m256i \_\_lasx_xvbitsel_v (\_\_m256i a, \_\_m256i b, \_\_m256i c)

### Synopsis

```
__m256i __lasx_xvbitsel_v (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvbitsel.v xr, xr, xr, xr
CPU Flags: LASX
```

### Description

Compute bitwise selection: for each bit position, if the bit in `c` equals to one, copy the bit from `b` to `dst`, otherwise copy from `a`.

### Examples

```
__m256i __lasx_xvbitsel_v(__m256i{0x1122334455667788, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, __m256i{0xabababababababab, 0x1234123443214321, 0x1234123443214321, 0x5678567856785678}, __m256i{0xffff0000aaaabbbb, 0x1111222233334444, 0x00000000ffffffff, 0xffffffff00000000})
= 0xabab3344ffeeefab 0x98ba9beccfedfb00 0xabcdef1243214321 0x56785678ddeeddee
```

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (c.dword[i] & b.dword[i]) | (~c.dword[i] & a.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 2                |
| 3C6000 | LA664 | 1       | 2                |

## \_\_m256i \_\_lasx_xvbitseli_b (\_\_m256i a, \_\_m256i b, imm0_255 imm)

### Synopsis

```
__m256i __lasx_xvbitseli_b (__m256i a, __m256i b, imm0_255 imm)
#include <lasxintrin.h>
Instruction: xvbitseli.b xr, xr, imm
CPU Flags: LASX
```

### Description

Compute bitwise selection: for each bit position, if the bit in `a` equals to one, copy the bit from `imm` to `dst`, otherwise copy from `b`.

### Examples

```
__m256i __lasx_xvbitseli_b( __m256i{ 0x1122334455667788, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, __m256i{ 0xabababababababab, 0x1234123443214321, 0x1234123443214321, 0x5678567856785678}, 0x12)
= 0xba8b9aabba8b9a23 0x1216123012031221 0x1230123653115311 0x5652565212121212
```

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = (~a.byte[i] & b.byte[i]) | (a.byte[i] & (u8)imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 2                |
| 3C6000 | LA664 | 1       | 2                |

## \_\_m256i \_\_lasx_xvbitclr_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvbitclr_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvbitclr.b xr, xr, xr
CPU Flags: LASX
```

### Description

Clear the bit specified by elements in `b` from 8-bit elements in `a`, save the result in `dst`.

### Examples

```
__m256i __lasx_xvbitclr_b(__m256i{0xffffffffffffffff, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, __m256i{0xabababababababab, 0x1234123443214321, 0x1234123443214321, 0x5678567856785678})
= 0xf7f7f7f7f7f7f7f7 0x99aabbccd5ecf700 0xabcdeb0212341234 0xaabaaaba9dee9dee
```

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = a.byte[i] & (~((u8)1 << (b.byte[i] % 8)));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvbitclr_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvbitclr_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvbitclr.h xr, xr, xr
CPU Flags: LASX
```

### Description

Clear the bit specified by elements in `b` from 16-bit elements in `a`, save the result in `dst`.

### Examples

```
__m256i __lasx_xvbitclr_h(__m256i{0xffffffffffffffff, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, __m256i{0xabababababababab, 0x1234123443214321, 0x1234123443214321, 0x5678567856785678})
= 0xf7fff7fff7fff7ff 0x99aabbccddecff00 0xabcdef0212341234 0xaabbaabbdceedcee
```

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = a.half[i] & (~((u16)1 << (b.half[i] % 16)));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvbitclr_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvbitclr_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvbitclr.w xr, xr, xr
CPU Flags: LASX
```

### Description

Clear the bit specified by elements in `b` from 32-bit elements in `a`, save the result in `dst`.

### Examples

```
__m256i __lasx_xvbitclr_w(__m256i{0xffffffffffffffff, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, __m256i{0xabababababababab, 0x1234123443214321, 0x1234123443214321, 0x5678567856785678})
= 0xfffff7fffffff7ff 0x99aabbccddeeff00 0xabcdef1212341234 0xaabbaabbdceeddee
```

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = a.word[i] & (~((u32)1 << (b.word[i] % 32)));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvbitclr_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvbitclr_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvbitclr.d xr, xr, xr
CPU Flags: LASX
```

### Description

Clear the bit specified by elements in `b` from 64-bit elements in `a`, save the result in `dst`.

### Examples

```
__m256i __lasx_xvbitclr_d(__m256i{0xffffffffffffffff, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, __m256i{0xabababababababab, 0x1234123443214321, 0x1234123443214321, 0x5678567856785678})
= 0xfffff7ffffffffff 0x99aabbccddeeff00 0xabcdef1012341234 0xaabbaabbddeeddee
```

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = a.dword[i] & (~((u64)1 << (b.dword[i] % 64)));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvbitclri_b (\_\_m256i a, imm0_7 imm)

### Synopsis

```
__m256i __lasx_xvbitclri_b (__m256i a, imm0_7 imm)
#include <lasxintrin.h>
Instruction: xvbitclri.b xr, xr, imm
CPU Flags: LASX
```

### Description

Clear the bit specified by `imm` from 8-bit elements in `a`, save the result in `dst`.

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

## \_\_m256i \_\_lasx_xvbitclri_h (\_\_m256i a, imm0_15 imm)

### Synopsis

```
__m256i __lasx_xvbitclri_h (__m256i a, imm0_15 imm)
#include <lasxintrin.h>
Instruction: xvbitclri.h xr, xr, imm
CPU Flags: LASX
```

### Description

Clear the bit specified by `imm` from 16-bit elements in `a`, save the result in `dst`.

### Examples

```
__m256i __lasx_xvbitclri_h( __m256i{ 0xffffffffffffffff, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, 1)
= 0xfffdfffdfffdfffd 0x99a8bbccddecff00 0xabcdef1012341234 0xaab9aab9ddecddec
```

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = a.half[i] & (~((u16)1 << imm));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvbitclri_w (\_\_m256i a, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvbitclri_w (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvbitclri.w xr, xr, imm
CPU Flags: LASX
```

### Description

Clear the bit specified by `imm` from 32-bit elements in `a`, save the result in `dst`.

### Examples

```
__m256i __lasx_xvbitclri_w( __m256i{ 0xffffffffffffffff, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, 1)
= 0xfffffffdfffffffd 0x99aabbccddeeff00 0xabcdef1012341234 0xaabbaab9ddeeddec
```

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = a.word[i] & (~((u32)1 << imm));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvbitclri_d (\_\_m256i a, imm0_63 imm)

### Synopsis

```
__m256i __lasx_xvbitclri_d (__m256i a, imm0_63 imm)
#include <lasxintrin.h>
Instruction: xvbitclri.d xr, xr, imm
CPU Flags: LASX
```

### Description

Clear the bit specified by `imm` from 64-bit elements in `a`, save the result in `dst`.

### Examples

```
__m256i __lasx_xvbitclri_d( __m256i{ 0xffffffffffffffff, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, 1)
= 0xfffffffffffffffd 0x99aabbccddeeff00 0xabcdef1212341234 0xaabbaabbddeeddec
```

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = a.dword[i] & (~((u64)1 << imm));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvbitset_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvbitset_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvbitset.b xr, xr, xr
CPU Flags: LASX
```

### Description

Set the bit specified by elements in `b` from 8-bit elements in `a`, save the result in `dst`.

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

## \_\_m256i \_\_lasx_xvbitset_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvbitset_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvbitset.h xr, xr, xr
CPU Flags: LASX
```

### Description

Set the bit specified by elements in `b` from 16-bit elements in `a`, save the result in `dst`.

### Examples

```
__m256i __lasx_xvbitset_h(__m256i{0x0000000000000000, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, __m256i{0xabababababababab, 0x1234123443214321, 0x1234123443214321, 0x5678567856785678})
= 0x0800080008000800 0x99babbdcddeeff02 0xabddef1212361236 0xabbbabbbddeeddee
```

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = a.half[i] | ((u16)1 << (b.half[i] % 16));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvbitset_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvbitset_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvbitset.w xr, xr, xr
CPU Flags: LASX
```

### Description

Set the bit specified by elements in `b` from 32-bit elements in `a`, save the result in `dst`.

### Examples

```
__m256i __lasx_xvbitset_w(__m256i{0x0000000000000000, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, __m256i{0xabababababababab, 0x1234123443214321, 0x1234123443214321, 0x5678567856785678})
= 0x0000080000000800 0x99babbccddeeff02 0xabddef1212341236 0xabbbaabbddeeddee
```

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = a.word[i] | ((u32)1 << (b.word[i] % 32));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvbitset_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvbitset_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvbitset.d xr, xr, xr
CPU Flags: LASX
```

### Description

Set the bit specified by elements in `b` from 64-bit elements in `a`, save the result in `dst`.

### Examples

```
__m256i __lasx_xvbitset_d(__m256i{0x0000000000000000, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, __m256i{0xabababababababab, 0x1234123443214321, 0x1234123443214321, 0x5678567856785678})
= 0x0000080000000000 0x99aabbceddeeff00 0xabcdef1212341234 0xabbbaabbddeeddee
```

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = a.dword[i] | ((u64)1 << (b.dword[i] % 64));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvbitseti_b (\_\_m256i a, imm0_7 imm)

### Synopsis

```
__m256i __lasx_xvbitseti_b (__m256i a, imm0_7 imm)
#include <lasxintrin.h>
Instruction: xvbitseti.b xr, xr, imm
CPU Flags: LASX
```

### Description

Set the bit specified by `imm` from 8-bit elements in `a`, save the result in `dst`.

### Examples

```
__m256i __lasx_xvbitseti_b( __m256i{ 0x0000000000000000, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, 1)
= 0x0202020202020202 0x9baabbcedfeeff02 0xabcfef1212361236 0xaabbaabbdfeedfee
```

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = a.byte[i] | ((u8)1 << imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvbitseti_h (\_\_m256i a, imm0_15 imm)

### Synopsis

```
__m256i __lasx_xvbitseti_h (__m256i a, imm0_15 imm)
#include <lasxintrin.h>
Instruction: xvbitseti.h xr, xr, imm
CPU Flags: LASX
```

### Description

Set the bit specified by `imm` from 16-bit elements in `a`, save the result in `dst`.

### Examples

```
__m256i __lasx_xvbitseti_h( __m256i{ 0x0000000000000000, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, 1)
= 0x0002000200020002 0x99aabbceddeeff02 0xabcfef1212361236 0xaabbaabbddeeddee
```

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = a.half[i] | ((u16)1 << imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvbitseti_w (\_\_m256i a, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvbitseti_w (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvbitseti.w xr, xr, imm
CPU Flags: LASX
```

### Description

Set the bit specified by `imm` from 32-bit elements in `a`, save the result in `dst`.

### Examples

```
__m256i __lasx_xvbitseti_w( __m256i{ 0x0000000000000000, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, 1)
= 0x0000000200000002 0x99aabbceddeeff02 0xabcdef1212341236 0xaabbaabbddeeddee
```

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = a.word[i] | ((u32)1 << imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvbitseti_d (\_\_m256i a, imm0_63 imm)

### Synopsis

```
__m256i __lasx_xvbitseti_d (__m256i a, imm0_63 imm)
#include <lasxintrin.h>
Instruction: xvbitseti.d xr, xr, imm
CPU Flags: LASX
```

### Description

Set the bit specified by `imm` from 64-bit elements in `a`, save the result in `dst`.

### Examples

```
__m256i __lasx_xvbitseti_d( __m256i{ 0x0000000000000000, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, 1)
= 0x0000000000000002 0x99aabbccddeeff02 0xabcdef1212341236 0xaabbaabbddeeddee
```

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = a.dword[i] | ((u64)1 << imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvbitrev_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvbitrev_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvbitrev.b xr, xr, xr
CPU Flags: LASX
```

### Description

Toggle the bit specified by elements in `b` from 8-bit elements in `a`, save the result in `dst`.

### Examples

```
__m256i __lasx_xvbitrev_b(__m256i{0x0f0f0f0f0f0f0f0f, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, __m256i{0xabababababababab, 0x1234123443214321, 0x1234123443214321, 0x5678567856785678})
= 0x0707070707070707 0x9dbabfdcd5ecf702 0xafddeb021a361a36 0xeabaeaba9def9def
```

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = a.byte[i] ^ ((u8)1 << (b.byte[i] % 8));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvbitrev_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvbitrev_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvbitrev.h xr, xr, xr
CPU Flags: LASX
```

### Description

Toggle the bit specified by elements in `b` from 16-bit elements in `a`, save the result in `dst`.

### Examples

```
__m256i __lasx_xvbitrev_h(__m256i{0x0f0f0f0f0f0f0f0f, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, __m256i{0xabababababababab, 0x1234123443214321, 0x1234123443214321, 0x5678567856785678})
= 0x070f070f070f070f 0x99babbdcddecff02 0xabddef0212361236 0xabbbabbbdceedcee
```

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = a.half[i] ^ ((u16)1 << (b.half[i] % 16));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvbitrev_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvbitrev_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvbitrev.w xr, xr, xr
CPU Flags: LASX
```

### Description

Toggle the bit specified by elements in `b` from 32-bit elements in `a`, save the result in `dst`.

### Examples

```
__m256i __lasx_xvbitrev_w(__m256i{0x0f0f0f0f0f0f0f0f, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, __m256i{0xabababababababab, 0x1234123443214321, 0x1234123443214321, 0x5678567856785678})
= 0x0f0f070f0f0f070f 0x99babbccddeeff02 0xabddef1212341236 0xabbbaabbdceeddee
```

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = a.word[i] ^ ((u32)1 << (b.word[i] % 32));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvbitrev_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvbitrev_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvbitrev.d xr, xr, xr
CPU Flags: LASX
```

### Description

Toggle the bit specified by elements in `b` from 64-bit elements in `a`, save the result in `dst`.

### Examples

```
__m256i __lasx_xvbitrev_d(__m256i{0x0f0f0f0f0f0f0f0f, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, __m256i{0xabababababababab, 0x1234123443214321, 0x1234123443214321, 0x5678567856785678})
= 0x0f0f070f0f0f0f0f 0x99aabbceddeeff00 0xabcdef1012341234 0xabbbaabbddeeddee
```

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = a.dword[i] ^ ((u64)1 << (b.dword[i] % 64));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvbitrevi_b (\_\_m256i a, imm0_7 imm)

### Synopsis

```
__m256i __lasx_xvbitrevi_b (__m256i a, imm0_7 imm)
#include <lasxintrin.h>
Instruction: xvbitrevi.b xr, xr, imm
CPU Flags: LASX
```

### Description

Toggle the bit specified by `imm` from 8-bit elements in `a`, save the result in `dst`.

### Examples

```
__m256i __lasx_xvbitrevi_b( __m256i{ 0x0f0f0f0f0f0f0f0f, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, 1)
= 0x0d0d0d0d0d0d0d0d 0x9ba8b9cedfecfd02 0xa9cfed1010361036 0xa8b9a8b9dfecdfec
```

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = a.byte[i] ^ ((u8)1 << imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvbitrevi_h (\_\_m256i a, imm0_15 imm)

### Synopsis

```
__m256i __lasx_xvbitrevi_h (__m256i a, imm0_15 imm)
#include <lasxintrin.h>
Instruction: xvbitrevi.h xr, xr, imm
CPU Flags: LASX
```

### Description

Toggle the bit specified by `imm` from 16-bit elements in `a`, save the result in `dst`.

### Examples

```
__m256i __lasx_xvbitrevi_h( __m256i{ 0x0f0f0f0f0f0f0f0f, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, 1)
= 0x0f0d0f0d0f0d0f0d 0x99a8bbceddecff02 0xabcfef1012361236 0xaab9aab9ddecddec
```

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = a.half[i] ^ ((u16)1 << imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvbitrevi_w (\_\_m256i a, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvbitrevi_w (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvbitrevi.w xr, xr, imm
CPU Flags: LASX
```

### Description

Toggle the bit specified by `imm` from 32-bit elements in `a`, save the result in `dst`.

### Examples

```
__m256i __lasx_xvbitrevi_w( __m256i{ 0x0f0f0f0f0f0f0f0f, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, 1)
= 0x0f0f0f0d0f0f0f0d 0x99aabbceddeeff02 0xabcdef1012341236 0xaabbaab9ddeeddec
```

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = a.word[i] ^ ((u32)1 << imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvbitrevi_d (\_\_m256i a, imm0_63 imm)

### Synopsis

```
__m256i __lasx_xvbitrevi_d (__m256i a, imm0_63 imm)
#include <lasxintrin.h>
Instruction: xvbitrevi.d xr, xr, imm
CPU Flags: LASX
```

### Description

Toggle the bit specified by `imm` from 64-bit elements in `a`, save the result in `dst`.

### Examples

```
__m256i __lasx_xvbitrevi_d( __m256i{ 0x0f0f0f0f0f0f0f0f, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, 1)
= 0x0f0f0f0f0f0f0f0d 0x99aabbccddeeff02 0xabcdef1212341236 0xaabbaabbddeeddec
```

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = a.dword[i] ^ ((u64)1 << imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvclo_b (\_\_m256i a)

### Synopsis

```
__m256i __lasx_xvclo_b (__m256i a)
#include <lasxintrin.h>
Instruction: xvclo.b xr, xr
CPU Flags: LASX
```

### Description

Count leading ones of 8-bit elements in `a`.

### Examples

```
__m256i __lasx_xvclo_b(__m256i{ 0x1122334455667788, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee})
= 0x0000000000000001 0x0101010202030800 0x0102030000000000 0x0101010102030203
```

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = clo(a.byte[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvclo_h (\_\_m256i a)

### Synopsis

```
__m256i __lasx_xvclo_h (__m256i a)
#include <lasxintrin.h>
Instruction: xvclo.h xr, xr
CPU Flags: LASX
```

### Description

Count leading ones of 16-bit elements in `a`.

### Examples

```
__m256i __lasx_xvclo_h(__m256i{ 0x1122334455667788, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee})
= 0x0000000000000000 0x0001000100020008 0x0001000300000000 0x0001000100020002
```

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = clo(a.half[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvclo_w (\_\_m256i a)

### Synopsis

```
__m256i __lasx_xvclo_w (__m256i a)
#include <lasxintrin.h>
Instruction: xvclo.w xr, xr
CPU Flags: LASX
```

### Description

Count leading ones of 32-bit elements in `a`.

### Examples

```
__m256i __lasx_xvclo_w(__m256i{ 0x1122334455667788, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee})
= 0x0000000000000000 0x0000000100000002 0x0000000100000000 0x0000000100000002
```

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = clo(a.word[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvclo_d (\_\_m256i a)

### Synopsis

```
__m256i __lasx_xvclo_d (__m256i a)
#include <lasxintrin.h>
Instruction: xvclo.d xr, xr
CPU Flags: LASX
```

### Description

Count leading ones of 64-bit elements in `a`.

### Examples

```
__m256i __lasx_xvclo_d(__m256i{ 0x1122334455667788, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee})
= 0x0000000000000000 0x0000000000000001 0x0000000000000001 0x0000000000000001
```

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = clo(a.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvclz_b (\_\_m256i a)

### Synopsis

```
__m256i __lasx_xvclz_b (__m256i a)
#include <lasxintrin.h>
Instruction: xvclz.b xr, xr
CPU Flags: LASX
```

### Description

Count leading zeros of 8-bit elements in `a`.

### Examples

```
__m256i __lasx_xvclz_b(__m256i{ 0x1122334455667788, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee})
= 0x0302020101010100 0x0000000000000008 0x0000000303020302 0x0000000000000000
```

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = clz(a.byte[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvclz_h (\_\_m256i a)

### Synopsis

```
__m256i __lasx_xvclz_h (__m256i a)
#include <lasxintrin.h>
Instruction: xvclz.h xr, xr
CPU Flags: LASX
```

### Description

Count leading zeros of 16-bit elements in `a`.

### Examples

```
__m256i __lasx_xvclz_h(__m256i{ 0x1122334455667788, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee})
= 0x0003000200010001 0x0000000000000000 0x0000000000030003 0x0000000000000000
```

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = clz(a.half[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvclz_w (\_\_m256i a)

### Synopsis

```
__m256i __lasx_xvclz_w (__m256i a)
#include <lasxintrin.h>
Instruction: xvclz.w xr, xr
CPU Flags: LASX
```

### Description

Count leading zeros of 32-bit elements in `a`.

### Examples

```
__m256i __lasx_xvclz_w(__m256i{ 0x1122334455667788, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee})
= 0x0000000300000001 0x0000000000000000 0x0000000000000003 0x0000000000000000
```

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = clz(a.word[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvclz_d (\_\_m256i a)

### Synopsis

```
__m256i __lasx_xvclz_d (__m256i a)
#include <lasxintrin.h>
Instruction: xvclz.d xr, xr
CPU Flags: LASX
```

### Description

Count leading zeros of 64-bit elements in `a`.

### Examples

```
__m256i __lasx_xvclz_d(__m256i{ 0x1122334455667788, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee})
= 0x0000000000000003 0x0000000000000000 0x0000000000000000 0x0000000000000000
```

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = clz(a.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvpcnt_b (\_\_m256i a)

### Synopsis

```
__m256i __lasx_xvpcnt_b (__m256i a)
#include <lasxintrin.h>
Instruction: xvpcnt.b xr, xr
CPU Flags: LASX
```

### Description

Count the number of ones (population, popcount) in 8-bit elements in `a`.

### Examples

```
__m256i __lasx_xvpcnt_b(__m256i{ 0x1122334455667788, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee})
= 0x0202040204040602 0x0404060406060800 0x0505070202030203 0x0406040606060606
```

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = popcount(a.byte[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvpcnt_h (\_\_m256i a)

### Synopsis

```
__m256i __lasx_xvpcnt_h (__m256i a)
#include <lasxintrin.h>
Instruction: xvpcnt.h xr, xr
CPU Flags: LASX
```

### Description

Count the number of ones (population, popcount) in 16-bit elements in `a`.

### Examples

```
__m256i __lasx_xvpcnt_h(__m256i{ 0x1122334455667788, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee})
= 0x0004000600080008 0x0008000a000c0008 0x000a000900050005 0x000a000a000c000c
```

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = popcount(a.half[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvpcnt_w (\_\_m256i a)

### Synopsis

```
__m256i __lasx_xvpcnt_w (__m256i a)
#include <lasxintrin.h>
Instruction: xvpcnt.w xr, xr
CPU Flags: LASX
```

### Description

Count the number of ones (population, popcount) in 32-bit elements in `a`.

### Examples

```
__m256i __lasx_xvpcnt_w(__m256i{ 0x1122334455667788, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee})
= 0x0000000a00000010 0x0000001200000014 0x000000130000000a 0x0000001400000018
```

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = popcount(a.word[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvpcnt_d (\_\_m256i a)

### Synopsis

```
__m256i __lasx_xvpcnt_d (__m256i a)
#include <lasxintrin.h>
Instruction: xvpcnt.d xr, xr
CPU Flags: LASX
```

### Description

Count the number of ones (population, popcount) in 64-bit elements in `a`.

### Examples

```
__m256i __lasx_xvpcnt_d(__m256i{ 0x1122334455667788, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee})
= 0x000000000000001a 0x0000000000000026 0x000000000000001d 0x000000000000002c
```

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = popcount(a.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |
