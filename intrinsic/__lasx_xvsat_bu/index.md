# \_\_lasx_xvsat_bu

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

### Synopsis

```
__m256i __lasx_xvsat_bu (__m256i a, imm0_7 imm)
#include <lasxintrin.h>
Instruction: xvsat.bu xr, xr, imm
CPU Flags: LASX
```

### Description

Saturate each unsigned 8-bit element in `a` to the number of significant bits specified by `imm`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = clamp<u8>(a.byte[i], 0, (1 << (imm + 1)) - 1);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
