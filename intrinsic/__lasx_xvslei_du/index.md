# \_\_lasx_xvslei_du

LASX intrinsic · group: [Integer Comparison](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_comparison/index.md)

### Synopsis

```
__m256i __lasx_xvslei_du (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvslei.du xr, xr, imm
CPU Flags: LASX
```

### Description

Compare the unsigned 64-bit elements in `a` and the zero-extended `imm`, store all-ones to `dst` if corresponding element in `a` is less than or equal to `imm`, zero otherwise.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = ((u64)a.dword[i] <= imm) ? 0xFFFFFFFFFFFFFFFF : 0;
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
