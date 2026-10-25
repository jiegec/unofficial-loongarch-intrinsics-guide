# \_\_lsx_vreplgr2vr_b

LSX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/misc/index.md)

### Synopsis

```
__m128i __lsx_vreplgr2vr_b (int val)
#include <lsxintrin.h>
Instruction: vreplgr2vr.b vr, r
CPU Flags: LSX
```

### Description

Broadcast the low 8 bits of `val` to all lanes of `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = val;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | N/A     | 1                |
| 3A6000   | LA664  | N/A     | 1                |
| 3C6000   | LA664  | N/A     | 1                |
| 2K1000LA | LA264  | N/A     | 0.5(1/2)         |
| 2K3000   | LA364E | N/A     | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
