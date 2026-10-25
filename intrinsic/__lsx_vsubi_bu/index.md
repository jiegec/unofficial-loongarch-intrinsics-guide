# \_\_lsx_vsubi_bu

LSX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_computation/index.md)

### Synopsis

```
__m128i __lsx_vsubi_bu (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vsubi.bu vr, vr, imm
CPU Flags: LSX
```

### Description

Subtract the zero-extended `imm` from each 8-bit element in `a`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = a.byte[i] - imm;
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

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
