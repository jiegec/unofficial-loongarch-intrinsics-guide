# \_\_lsx_vbitclri_b

LSX intrinsic · group: [Bitwise Operations](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/bitwise_operations/index.md)

### Synopsis

```
__m128i __lsx_vbitclri_b (__m128i a, imm0_7 imm)
#include <lsxintrin.h>
Instruction: vbitclri.b vr, vr, imm
CPU Flags: LSX
```

### Description

For each 8-bit element, clear the bit indexed by `imm`, save the result in `dst`.

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

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
