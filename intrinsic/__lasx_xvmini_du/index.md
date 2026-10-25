# \_\_lasx_xvmini_du

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvmini_du (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvmini.du xr, xr, imm
CPU Flags: LASX
```

### Description

Compute elementwise minimum for unsigned 64-bit elements in `a` and the zero-extended `imm`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = min((u64)a.dword[i], (u64)imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
