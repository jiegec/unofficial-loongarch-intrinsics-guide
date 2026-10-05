# Bitwise Operations

## \_\_m128i \_\_lsx_vbitsel_v (\_\_m128i a, \_\_m128i b, \_\_m128i c)

### Synopsis

```
__m128i __lsx_vbitsel_v (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vbitsel.v vr, vr, vr, vr
CPU Flags: LSX
```

### Description

Compute bitwise selection: for each bit position, if the bit in `c` equals to one, copy the bit from `b` to `dst`, otherwise copy from `a`.

### Examples

```
__m128i __lsx_vbitsel_v(__m128i{0x1122334455667788, 0x99aabbccddeeff00}, __m128i{0xabababababababab, 0x1234123443214321}, __m128i{0xffff0000aaaabbbb, 0x1111222233334444})
= 0xabab3344ffeeefab 0x98ba9beccfedfb00
```

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (c.dword[i] & b.dword[i]) | (~c.dword[i] & a.dword[i]);
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

## \_\_m128i \_\_lsx_vbitseli_b (\_\_m128i a, \_\_m128i b, imm0_255 imm)

### Synopsis

```
__m128i __lsx_vbitseli_b (__m128i a, __m128i b, imm0_255 imm)
#include <lsxintrin.h>
Instruction: vbitseli.b vr, vr, imm
CPU Flags: LSX
```

### Description

Compute bitwise selection: for each bit position, if the bit in `a` equals to one, copy the bit from `imm` to `dst`, otherwise copy from `b`.

### Examples

```
__m128i __lsx_vbitseli_b(__m128i{0x1122334455667788, 0x99aabbccddeeff00}, __m128i{0xabababababababab, 0x1234123443214321}, 0x12)
= 0xba8b9aabba8b9a23 0x1216123012031221
```

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = (~a.byte[i] & b.byte[i]) | (a.byte[i] & (u8)imm);
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

## \_\_m128i \_\_lsx_vbitclr_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vbitclr_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vbitclr.b vr, vr, vr
CPU Flags: LSX
```

### Description

Clear the bit specified by elements in `b` from 8-bit elements in `a`, save the result in `dst`.

### Examples

```
__m128i __lsx_vbitclr_b(__m128i{0xffffffffffffffff, 0x99aabbccddeeff00}, __m128i{0xabababababababab, 0x1234123443214321})
= 0xf7f7f7f7f7f7f7f7 0x99aabbccd5ecf700
```

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = a.byte[i] & (~((u8)1 << (b.byte[i] % 8)));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vbitclr_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vbitclr_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vbitclr.h vr, vr, vr
CPU Flags: LSX
```

### Description

Clear the bit specified by elements in `b` from 16-bit elements in `a`, save the result in `dst`.

### Examples

```
__m128i __lsx_vbitclr_h(__m128i{0xffffffffffffffff, 0x99aabbccddeeff00}, __m128i{0xabababababababab, 0x1234123443214321})
= 0xf7fff7fff7fff7ff 0x99aabbccddecff00
```

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = a.half[i] & (~((u16)1 << (b.half[i] % 16)));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vbitclr_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vbitclr_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vbitclr.w vr, vr, vr
CPU Flags: LSX
```

### Description

Clear the bit specified by elements in `b` from 32-bit elements in `a`, save the result in `dst`.

### Examples

```
__m128i __lsx_vbitclr_w(__m128i{0xffffffffffffffff, 0x99aabbccddeeff00}, __m128i{0xabababababababab, 0x1234123443214321})
= 0xfffff7fffffff7ff 0x99aabbccddeeff00
```

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = a.word[i] & (~((u32)1 << (b.word[i] % 32)));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vbitclr_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vbitclr_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vbitclr.d vr, vr, vr
CPU Flags: LSX
```

### Description

Clear the bit specified by elements in `b` from 64-bit elements in `a`, save the result in `dst`.

### Examples

```
__m128i __lsx_vbitclr_d(__m128i{0xffffffffffffffff, 0x99aabbccddeeff00}, __m128i{0xabababababababab, 0x1234123443214321})
= 0xfffff7ffffffffff 0x99aabbccddeeff00
```

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = a.dword[i] & (~((u64)1 << (b.dword[i] % 64)));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vbitclri_b (\_\_m128i a, imm0_7 imm)

### Synopsis

```
__m128i __lsx_vbitclri_b (__m128i a, imm0_7 imm)
#include <lsxintrin.h>
Instruction: vbitclri.b vr, vr, imm
CPU Flags: LSX
```

### Description

Clear the bit specified by `imm` from 8-bit elements in `a`, save the result in `dst`.

### Examples

```
__m128i __lsx_vbitclri_b(__m128i{0xffffffffffffffff, 0x99aabbccddeeff00}, 1)
= 0xfdfdfdfdfdfdfdfd 0x99a8b9ccddecfd00
```

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = a.byte[i] & (~((u8)1 << imm));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vbitclri_h (\_\_m128i a, imm0_15 imm)

### Synopsis

```
__m128i __lsx_vbitclri_h (__m128i a, imm0_15 imm)
#include <lsxintrin.h>
Instruction: vbitclri.h vr, vr, imm
CPU Flags: LSX
```

### Description

Clear the bit specified by `imm` from 16-bit elements in `a`, save the result in `dst`.

### Examples

```
__m128i __lsx_vbitclri_h(__m128i{0xffffffffffffffff, 0x99aabbccddeeff00}, 1)
= 0xfffdfffdfffdfffd 0x99a8bbccddecff00
```

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = a.half[i] & (~((u16)1 << imm));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vbitclri_w (\_\_m128i a, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vbitclri_w (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vbitclri.w vr, vr, imm
CPU Flags: LSX
```

### Description

Clear the bit specified by `imm` from 32-bit elements in `a`, save the result in `dst`.

### Examples

```
__m128i __lsx_vbitclri_w(__m128i{0xffffffffffffffff, 0x99aabbccddeeff00}, 1)
= 0xfffffffdfffffffd 0x99aabbccddeeff00
```

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = a.word[i] & (~((u32)1 << imm));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vbitclri_d (\_\_m128i a, imm0_63 imm)

### Synopsis

```
__m128i __lsx_vbitclri_d (__m128i a, imm0_63 imm)
#include <lsxintrin.h>
Instruction: vbitclri.d vr, vr, imm
CPU Flags: LSX
```

### Description

Clear the bit specified by `imm` from 64-bit elements in `a`, save the result in `dst`.

### Examples

```
__m128i __lsx_vbitclri_d(__m128i{0xffffffffffffffff, 0x99aabbccddeeff00}, 1)
= 0xfffffffffffffffd 0x99aabbccddeeff00
```

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = a.dword[i] & (~((u64)1 << imm));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vbitset_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vbitset_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vbitset.b vr, vr, vr
CPU Flags: LSX
```

### Description

Set the bit specified by elements in `b` from 8-bit elements in `a`, save the result in `dst`.

### Examples

```
__m128i __lsx_vbitset_b(__m128i{0x0000000000000000, 0x99aabbccddeeff00}, __m128i{0xabababababababab, 0x1234123443214321})
= 0x0808080808080808 0x9dbabfdcddeeff02
```

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = a.byte[i] | ((u8)1 << (b.byte[i] % 8));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vbitset_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vbitset_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vbitset.h vr, vr, vr
CPU Flags: LSX
```

### Description

Set the bit specified by elements in `b` from 16-bit elements in `a`, save the result in `dst`.

### Examples

```
__m128i __lsx_vbitset_h(__m128i{0x0000000000000000, 0x99aabbccddeeff00}, __m128i{0xabababababababab, 0x1234123443214321})
= 0x0800080008000800 0x99babbdcddeeff02
```

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = a.half[i] | ((u16)1 << (b.half[i] % 16));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vbitset_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vbitset_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vbitset.w vr, vr, vr
CPU Flags: LSX
```

### Description

Set the bit specified by elements in `b` from 32-bit elements in `a`, save the result in `dst`.

### Examples

```
__m128i __lsx_vbitset_w(__m128i{0x0000000000000000, 0x99aabbccddeeff00}, __m128i{0xabababababababab, 0x1234123443214321})
= 0x0000080000000800 0x99babbccddeeff02
```

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = a.word[i] | ((u32)1 << (b.word[i] % 32));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vbitset_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vbitset_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vbitset.d vr, vr, vr
CPU Flags: LSX
```

### Description

Set the bit specified by elements in `b` from 64-bit elements in `a`, save the result in `dst`.

### Examples

```
__m128i __lsx_vbitset_d(__m128i{0x0000000000000000, 0x99aabbccddeeff00}, __m128i{0xabababababababab, 0x1234123443214321})
= 0x0000080000000000 0x99aabbceddeeff00
```

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = a.dword[i] | ((u64)1 << (b.dword[i] % 64));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vbitseti_b (\_\_m128i a, imm0_7 imm)

### Synopsis

```
__m128i __lsx_vbitseti_b (__m128i a, imm0_7 imm)
#include <lsxintrin.h>
Instruction: vbitseti.b vr, vr, imm
CPU Flags: LSX
```

### Description

Set the bit specified by `imm` from 8-bit elements in `a`, save the result in `dst`.

### Examples

```
__m128i __lsx_vbitseti_b(__m128i{0x0000000000000000, 0x99aabbccddeeff00}, 1)
= 0x0202020202020202 0x9baabbcedfeeff02
```

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = a.byte[i] | ((u8)1 << imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vbitseti_h (\_\_m128i a, imm0_15 imm)

### Synopsis

```
__m128i __lsx_vbitseti_h (__m128i a, imm0_15 imm)
#include <lsxintrin.h>
Instruction: vbitseti.h vr, vr, imm
CPU Flags: LSX
```

### Description

Set the bit specified by `imm` from 16-bit elements in `a`, save the result in `dst`.

### Examples

```
__m128i __lsx_vbitseti_h(__m128i{0x0000000000000000, 0x99aabbccddeeff00}, 1)
= 0x0002000200020002 0x99aabbceddeeff02
```

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = a.half[i] | ((u16)1 << imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vbitseti_w (\_\_m128i a, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vbitseti_w (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vbitseti.w vr, vr, imm
CPU Flags: LSX
```

### Description

Set the bit specified by `imm` from 32-bit elements in `a`, save the result in `dst`.

### Examples

```
__m128i __lsx_vbitseti_w(__m128i{0x0000000000000000, 0x99aabbccddeeff00}, 1)
= 0x0000000200000002 0x99aabbceddeeff02
```

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = a.word[i] | ((u32)1 << imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vbitseti_d (\_\_m128i a, imm0_63 imm)

### Synopsis

```
__m128i __lsx_vbitseti_d (__m128i a, imm0_63 imm)
#include <lsxintrin.h>
Instruction: vbitseti.d vr, vr, imm
CPU Flags: LSX
```

### Description

Set the bit specified by `imm` from 64-bit elements in `a`, save the result in `dst`.

### Examples

```
__m128i __lsx_vbitseti_d(__m128i{0x0000000000000000, 0x99aabbccddeeff00}, 1)
= 0x0000000000000002 0x99aabbccddeeff02
```

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = a.dword[i] | ((u64)1 << imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vbitrev_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vbitrev_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vbitrev.b vr, vr, vr
CPU Flags: LSX
```

### Description

Toggle the bit specified by elements in `b` from 8-bit elements in `a`, save the result in `dst`.

### Examples

```
__m128i __lsx_vbitrev_b(__m128i{0x0f0f0f0f0f0f0f0f, 0x99aabbccddeeff00}, __m128i{0xabababababababab, 0x1234123443214321})
= 0x0707070707070707 0x9dbabfdcd5ecf702
```

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = a.byte[i] ^ ((u8)1 << (b.byte[i] % 8));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vbitrev_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vbitrev_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vbitrev.h vr, vr, vr
CPU Flags: LSX
```

### Description

Toggle the bit specified by elements in `b` from 16-bit elements in `a`, save the result in `dst`.

### Examples

```
__m128i __lsx_vbitrev_h(__m128i{0x0f0f0f0f0f0f0f0f, 0x99aabbccddeeff00}, __m128i{0xabababababababab, 0x1234123443214321})
= 0x070f070f070f070f 0x99babbdcddecff02
```

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = a.half[i] ^ ((u16)1 << (b.half[i] % 16));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vbitrev_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vbitrev_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vbitrev.w vr, vr, vr
CPU Flags: LSX
```

### Description

Toggle the bit specified by elements in `b` from 32-bit elements in `a`, save the result in `dst`.

### Examples

```
__m128i __lsx_vbitrev_w(__m128i{0x0f0f0f0f0f0f0f0f, 0x99aabbccddeeff00}, __m128i{0xabababababababab, 0x1234123443214321})
= 0x0f0f070f0f0f070f 0x99babbccddeeff02
```

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = a.word[i] ^ ((u32)1 << (b.word[i] % 32));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vbitrev_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vbitrev_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vbitrev.d vr, vr, vr
CPU Flags: LSX
```

### Description

Toggle the bit specified by elements in `b` from 64-bit elements in `a`, save the result in `dst`.

### Examples

```
__m128i __lsx_vbitrev_d(__m128i{0x0f0f0f0f0f0f0f0f, 0x99aabbccddeeff00}, __m128i{0xabababababababab, 0x1234123443214321})
= 0x0f0f070f0f0f0f0f 0x99aabbceddeeff00
```

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = a.dword[i] ^ ((u64)1 << (b.dword[i] % 64));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vbitrevi_b (\_\_m128i a, imm0_7 imm)

### Synopsis

```
__m128i __lsx_vbitrevi_b (__m128i a, imm0_7 imm)
#include <lsxintrin.h>
Instruction: vbitrevi.b vr, vr, imm
CPU Flags: LSX
```

### Description

Toggle the bit specified by `imm` from 8-bit elements in `a`, save the result in `dst`.

### Examples

```
__m128i __lsx_vbitrevi_b(__m128i{0x0f0f0f0f0f0f0f0f, 0x99aabbccddeeff00}, 1)
= 0x0d0d0d0d0d0d0d0d 0x9ba8b9cedfecfd02
```

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = a.byte[i] ^ ((u8)1 << imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vbitrevi_h (\_\_m128i a, imm0_15 imm)

### Synopsis

```
__m128i __lsx_vbitrevi_h (__m128i a, imm0_15 imm)
#include <lsxintrin.h>
Instruction: vbitrevi.h vr, vr, imm
CPU Flags: LSX
```

### Description

Toggle the bit specified by `imm` from 16-bit elements in `a`, save the result in `dst`.

### Examples

```
__m128i __lsx_vbitrevi_h(__m128i{0x0f0f0f0f0f0f0f0f, 0x99aabbccddeeff00}, 1)
= 0x0f0d0f0d0f0d0f0d 0x99a8bbceddecff02
```

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = a.half[i] ^ ((u16)1 << imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vbitrevi_w (\_\_m128i a, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vbitrevi_w (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vbitrevi.w vr, vr, imm
CPU Flags: LSX
```

### Description

Toggle the bit specified by `imm` from 32-bit elements in `a`, save the result in `dst`.

### Examples

```
__m128i __lsx_vbitrevi_w(__m128i{0x0f0f0f0f0f0f0f0f, 0x99aabbccddeeff00}, 1)
= 0x0f0f0f0d0f0f0f0d 0x99aabbceddeeff02
```

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = a.word[i] ^ ((u32)1 << imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vbitrevi_d (\_\_m128i a, imm0_63 imm)

### Synopsis

```
__m128i __lsx_vbitrevi_d (__m128i a, imm0_63 imm)
#include <lsxintrin.h>
Instruction: vbitrevi.d vr, vr, imm
CPU Flags: LSX
```

### Description

Toggle the bit specified by `imm` from 64-bit elements in `a`, save the result in `dst`.

### Examples

```
__m128i __lsx_vbitrevi_d(__m128i{0x0f0f0f0f0f0f0f0f, 0x99aabbccddeeff00}, 1)
= 0x0f0f0f0f0f0f0f0d 0x99aabbccddeeff02
```

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = a.dword[i] ^ ((u64)1 << imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vclo_b (\_\_m128i a)

### Synopsis

```
__m128i __lsx_vclo_b (__m128i a)
#include <lsxintrin.h>
Instruction: vclo.b vr, vr
CPU Flags: LSX
```

### Description

Count leading ones of 8-bit elements in `a`.

### Examples

```
__m128i __lsx_vclo_b(__m128i{0x1122334455667788, 0x99aabbccddeeff00})
= 0x0000000000000001 0x0101010202030800
```

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = clo(a.byte[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vclo_h (\_\_m128i a)

### Synopsis

```
__m128i __lsx_vclo_h (__m128i a)
#include <lsxintrin.h>
Instruction: vclo.h vr, vr
CPU Flags: LSX
```

### Description

Count leading ones of 16-bit elements in `a`.

### Examples

```
__m128i __lsx_vclo_h(__m128i{0x1122334455667788, 0x99aabbccddeeff00})
= 0x0000000000000000 0x0001000100020008
```

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = clo(a.half[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vclo_w (\_\_m128i a)

### Synopsis

```
__m128i __lsx_vclo_w (__m128i a)
#include <lsxintrin.h>
Instruction: vclo.w vr, vr
CPU Flags: LSX
```

### Description

Count leading ones of 32-bit elements in `a`.

### Examples

```
__m128i __lsx_vclo_w(__m128i{0x1122334455667788, 0x99aabbccddeeff00})
= 0x0000000000000000 0x0000000100000002
```

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = clo(a.word[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vclo_d (\_\_m128i a)

### Synopsis

```
__m128i __lsx_vclo_d (__m128i a)
#include <lsxintrin.h>
Instruction: vclo.d vr, vr
CPU Flags: LSX
```

### Description

Count leading ones of 64-bit elements in `a`.

### Examples

```
__m128i __lsx_vclo_d(__m128i{0x1122334455667788, 0x99aabbccddeeff00})
= 0x0000000000000000 0x0000000000000001
```

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = clo(a.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vclz_b (\_\_m128i a)

### Synopsis

```
__m128i __lsx_vclz_b (__m128i a)
#include <lsxintrin.h>
Instruction: vclz.b vr, vr
CPU Flags: LSX
```

### Description

Count leading zeros of 8-bit elements in `a`.

### Examples

```
__m128i __lsx_vclz_b(__m128i{0x1122334455667788, 0x99aabbccddeeff00})
= 0x0302020101010100 0x0000000000000008
```

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = clz(a.byte[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vclz_h (\_\_m128i a)

### Synopsis

```
__m128i __lsx_vclz_h (__m128i a)
#include <lsxintrin.h>
Instruction: vclz.h vr, vr
CPU Flags: LSX
```

### Description

Count leading zeros of 16-bit elements in `a`.

### Examples

```
__m128i __lsx_vclz_h(__m128i{0x1122334455667788, 0x99aabbccddeeff00})
= 0x0003000200010001 0x0000000000000000
```

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = clz(a.half[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vclz_w (\_\_m128i a)

### Synopsis

```
__m128i __lsx_vclz_w (__m128i a)
#include <lsxintrin.h>
Instruction: vclz.w vr, vr
CPU Flags: LSX
```

### Description

Count leading zeros of 32-bit elements in `a`.

### Examples

```
__m128i __lsx_vclz_w(__m128i{0x1122334455667788, 0x99aabbccddeeff00})
= 0x0000000300000001 0x0000000000000000
```

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = clz(a.word[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vclz_d (\_\_m128i a)

### Synopsis

```
__m128i __lsx_vclz_d (__m128i a)
#include <lsxintrin.h>
Instruction: vclz.d vr, vr
CPU Flags: LSX
```

### Description

Count leading zeros of 64-bit elements in `a`.

### Examples

```
__m128i __lsx_vclz_d(__m128i{0x1122334455667788, 0x99aabbccddeeff00})
= 0x0000000000000003 0x0000000000000000
```

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = clz(a.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vpcnt_b (\_\_m128i a)

### Synopsis

```
__m128i __lsx_vpcnt_b (__m128i a)
#include <lsxintrin.h>
Instruction: vpcnt.b vr, vr
CPU Flags: LSX
```

### Description

Count the number of ones (population, popcount) in 8-bit elements in `a`.

### Examples

```
__m128i __lsx_vpcnt_b(__m128i{0x1122334455667788, 0x99aabbccddeeff00})
= 0x0202040204040602 0x0404060406060800
```

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = popcount(a.byte[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vpcnt_h (\_\_m128i a)

### Synopsis

```
__m128i __lsx_vpcnt_h (__m128i a)
#include <lsxintrin.h>
Instruction: vpcnt.h vr, vr
CPU Flags: LSX
```

### Description

Count the number of ones (population, popcount) in 16-bit elements in `a`.

### Examples

```
__m128i __lsx_vpcnt_h(__m128i{0x1122334455667788, 0x99aabbccddeeff00})
= 0x0004000600080008 0x0008000a000c0008
```

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = popcount(a.half[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vpcnt_w (\_\_m128i a)

### Synopsis

```
__m128i __lsx_vpcnt_w (__m128i a)
#include <lsxintrin.h>
Instruction: vpcnt.w vr, vr
CPU Flags: LSX
```

### Description

Count the number of ones (population, popcount) in 32-bit elements in `a`.

### Examples

```
__m128i __lsx_vpcnt_w(__m128i{0x1122334455667788, 0x99aabbccddeeff00})
= 0x0000000a00000010 0x0000001200000014
```

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = popcount(a.word[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vpcnt_d (\_\_m128i a)

### Synopsis

```
__m128i __lsx_vpcnt_d (__m128i a)
#include <lsxintrin.h>
Instruction: vpcnt.d vr, vr
CPU Flags: LSX
```

### Description

Count the number of ones (population, popcount) in 64-bit elements in `a`.

### Examples

```
__m128i __lsx_vpcnt_d(__m128i{0x1122334455667788, 0x99aabbccddeeff00})
= 0x000000000000001a 0x0000000000000026
```

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = popcount(a.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |
