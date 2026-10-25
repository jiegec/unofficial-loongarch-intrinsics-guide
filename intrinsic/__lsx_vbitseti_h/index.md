# \_\_lsx_vbitseti_h

LSX intrinsic · group: [Bitwise Operations](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/bitwise_operations/index.md)

### Synopsis

```
__m128i __lsx_vbitseti_h (__m128i a, imm0_15 imm)
#include <lsxintrin.h>
Instruction: vbitseti.h vr, vr, imm
CPU Flags: LSX
```

### Description

For each 16-bit element, set the bit indexed by `imm`, save the result in `dst`.

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

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
