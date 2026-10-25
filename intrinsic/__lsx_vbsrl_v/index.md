# \_\_lsx_vbsrl_v

LSX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/shift/index.md)

### Synopsis

```
__m128i __lsx_vbsrl_v (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vbsrl.v vr, vr, imm
CPU Flags: LSX
```

### Description

Shift each 128-bit lane of `a` right by `(imm % 16) * 8` bits, filling the vacated bits with zeros.

### Operation

```
int shift = (imm * 8) % 128;
dst.qword[0] = (u128)a.qword[0] >> shift;
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
