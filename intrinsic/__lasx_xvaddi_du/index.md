# \_\_lasx_xvaddi_du

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvaddi_du (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvaddi.du xr, xr, imm
CPU Flags: LASX
```

### Description

Add the zero-extended `imm` to each 64-bit element in `a`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = a.dword[i] + imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
