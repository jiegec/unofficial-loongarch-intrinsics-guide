# \_\_lasx_xvreplve0_b

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

### Synopsis

```
__m256i __lasx_xvreplve0_b (__m256i a)
#include <lasxintrin.h>
Instruction: xvreplve0.b xr, xr
CPU Flags: LASX
```

### Description

Repeat the first 8-bit lane from `a` to all lanes of `dst`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = a.byte[0];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 4                |
| 3C6000 | LA664 | 3       | 4                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
