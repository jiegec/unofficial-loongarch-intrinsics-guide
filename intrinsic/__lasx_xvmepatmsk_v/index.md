# \_\_lasx_xvmepatmsk_v

LASX intrinsic · group: [Undocumented Intrinsics](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/undocumented/index.md)

### Synopsis

```
__m256i __lasx_xvmepatmsk_v (int mode, int uimm5)
#include <lasxintrin.h>
Instruction: xvmepatmsk.v xr, mode, uimm5
CPU Flags: LASX
```

### Description

Compute an 8-bit pattern selected by `mode` (0: repeating 0..3; 1: 0..3 shifted by 1 for each group; 2: 4..7 shifted by 1 for each group; 3: 0..15), then add `uimm5` to each byte element.

### Examples

```
__m256i __lasx_xvmepatmsk_v(3, 1)
= 0x0807060504030201 0x100f0e0d0c0b0a09 0x0807060504030201 0x100f0e0d0c0b0a09
```

### Operation

```
if (mode == 0b00) {
  for (int i = 0; i < 16; i++) {
    dst.byte[i + 16] = dst.byte[i] =
        uimm5 + (i % 4); // [0 1 2 3 0 1 2 3 0 1 2 3 0 1 2 3]
  }
} else if (mode == 0b01) {
  for (int i = 0; i < 16; i++) {
    dst.byte[i + 16] = dst.byte[i] =
        uimm5 + (i / 4) + (i % 4); // [0 1 2 3 1 2 3 4 2 3 4 5 3 4 5 6]
  }
} else if (mode == 0b10) {
  for (int i = 0; i < 16; i++) {
    dst.byte[i + 16] = dst.byte[i] =
        uimm5 + (i / 4) + (i % 4) + 4; // [4 5 6 7 5 6 7 8 6 7 8 9 7 8 9 10]
  }
} else if (mode == 0b11) {
  for (int i = 0; i < 16; i++) {
    dst.byte[i + 16] = dst.byte[i] =
        uimm5 + i; // [0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15]
  }
} else {
  // illegal instruction
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | N/A     | 2                |
| 3C6000 | LA664 | N/A     | 4                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
