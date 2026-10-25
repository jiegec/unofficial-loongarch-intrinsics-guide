# Branch

## int \_\_lsx_bz_v (\_\_m128i a)

### Synopsis

```
int __lsx_bz_v (__m128i a)
#include <lsxintrin.h>
Instruction: vseteqz.v fcc, vr; bcnez
CPU Flags: LSX
```

### Description

Expected to be used in branches: branch if the whole vector `a` equals to zero.

### Operation

```
dst = a.qword[0] == 0;
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | N/A     | 2                |
| 3A6000   | LA664  | N/A     | 2                |
| 3C6000   | LA664  | N/A     | 2                |
| 2K1000LA | LA264  | N/A     | 1                |
| 2K3000   | LA364E | N/A     | 1                |

## int \_\_lsx_bnz_v (\_\_m128i a)

### Synopsis

```
int __lsx_bnz_v (__m128i a)
#include <lsxintrin.h>
Instruction: vsetnez.v fcc, vr; bcnez
CPU Flags: LSX
```

### Description

Expected to be used in branches: branch if the whole vector `a` is non-zero.

### Operation

```
dst = a.qword[0] != 0;
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | N/A     | 2                |
| 3A6000   | LA664  | N/A     | 2                |
| 3C6000   | LA664  | N/A     | 2                |
| 2K1000LA | LA264  | N/A     | 1                |
| 2K3000   | LA364E | N/A     | 1                |

## int \_\_lsx_bz_b (\_\_m128i a)

### Synopsis

```
int __lsx_bz_b (__m128i a)
#include <lsxintrin.h>
Instruction: vsetanyeqz.b fcc, vr; bcnez
CPU Flags: LSX
```

### Description

Expected to be used in branches: branch if any 8-bit element in `a` equals to zero.

### Operation

```
dst = 0;
for (int i = 0; i < 16; i++) {
  if (a.byte[i] == 0) {
    dst = 1;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | N/A     | 2                |
| 3A6000   | LA664  | N/A     | 2                |
| 3C6000   | LA664  | N/A     | 2                |
| 2K1000LA | LA264  | N/A     | 1                |
| 2K3000   | LA364E | N/A     | 1                |

## int \_\_lsx_bz_h (\_\_m128i a)

### Synopsis

```
int __lsx_bz_h (__m128i a)
#include <lsxintrin.h>
Instruction: vsetanyeqz.h fcc, vr; bcnez
CPU Flags: LSX
```

### Description

Expected to be used in branches: branch if any 16-bit element in `a` equals to zero.

### Operation

```
dst = 0;
for (int i = 0; i < 8; i++) {
  if (a.half[i] == 0) {
    dst = 1;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | N/A     | 2                |
| 3A6000   | LA664  | N/A     | 2                |
| 3C6000   | LA664  | N/A     | 2                |
| 2K1000LA | LA264  | N/A     | 1                |
| 2K3000   | LA364E | N/A     | 1                |

## int \_\_lsx_bz_w (\_\_m128i a)

### Synopsis

```
int __lsx_bz_w (__m128i a)
#include <lsxintrin.h>
Instruction: vsetanyeqz.w fcc, vr; bcnez
CPU Flags: LSX
```

### Description

Expected to be used in branches: branch if any 32-bit element in `a` equals to zero.

### Operation

```
dst = 0;
for (int i = 0; i < 4; i++) {
  if (a.word[i] == 0) {
    dst = 1;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | N/A     | 2                |
| 3A6000   | LA664  | N/A     | 2                |
| 3C6000   | LA664  | N/A     | 2                |
| 2K1000LA | LA264  | N/A     | 1                |
| 2K3000   | LA364E | N/A     | 1                |

## int \_\_lsx_bz_d (\_\_m128i a)

### Synopsis

```
int __lsx_bz_d (__m128i a)
#include <lsxintrin.h>
Instruction: vsetanyeqz.d fcc, vr; bcnez
CPU Flags: LSX
```

### Description

Expected to be used in branches: branch if any 64-bit element in `a` equals to zero.

### Operation

```
dst = 0;
for (int i = 0; i < 2; i++) {
  if (a.dword[i] == 0) {
    dst = 1;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | N/A     | 2                |
| 3A6000   | LA664  | N/A     | 2                |
| 3C6000   | LA664  | N/A     | 2                |
| 2K1000LA | LA264  | N/A     | 1                |
| 2K3000   | LA364E | N/A     | 1                |

## int \_\_lsx_bnz_b (\_\_m128i a)

### Synopsis

```
int __lsx_bnz_b (__m128i a)
#include <lsxintrin.h>
Instruction: vsetallnez.b fcc, vr; bcnez
CPU Flags: LSX
```

### Description

Expected to be used in branches: branch if all 8-bit elements in `a` are non-zero.

### Operation

```
dst = 1;
for (int i = 0; i < 16; i++) {
  if (a.byte[i] == 0) {
    dst = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | N/A     | 2                |
| 3A6000   | LA664  | N/A     | 2                |
| 3C6000   | LA664  | N/A     | 2                |
| 2K1000LA | LA264  | N/A     | 1                |
| 2K3000   | LA364E | N/A     | 1                |

## int \_\_lsx_bnz_h (\_\_m128i a)

### Synopsis

```
int __lsx_bnz_h (__m128i a)
#include <lsxintrin.h>
Instruction: vsetallnez.h fcc, vr; bcnez
CPU Flags: LSX
```

### Description

Expected to be used in branches: branch if all 16-bit elements in `a` are non-zero.

### Operation

```
dst = 1;
for (int i = 0; i < 8; i++) {
  if (a.half[i] == 0) {
    dst = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | N/A     | 2                |
| 3A6000   | LA664  | N/A     | 2                |
| 3C6000   | LA664  | N/A     | 2                |
| 2K1000LA | LA264  | N/A     | 1                |
| 2K3000   | LA364E | N/A     | 1                |

## int \_\_lsx_bnz_w (\_\_m128i a)

### Synopsis

```
int __lsx_bnz_w (__m128i a)
#include <lsxintrin.h>
Instruction: vsetallnez.w fcc, vr; bcnez
CPU Flags: LSX
```

### Description

Expected to be used in branches: branch if all 32-bit elements in `a` are non-zero.

### Operation

```
dst = 1;
for (int i = 0; i < 4; i++) {
  if (a.word[i] == 0) {
    dst = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | N/A     | 2                |
| 3A6000   | LA664  | N/A     | 2                |
| 3C6000   | LA664  | N/A     | 2                |
| 2K1000LA | LA264  | N/A     | 1                |
| 2K3000   | LA364E | N/A     | 1                |

## int \_\_lsx_bnz_d (\_\_m128i a)

### Synopsis

```
int __lsx_bnz_d (__m128i a)
#include <lsxintrin.h>
Instruction: vsetallnez.d fcc, vr; bcnez
CPU Flags: LSX
```

### Description

Expected to be used in branches: branch if all 64-bit elements in `a` are non-zero.

### Operation

```
dst = 1;
for (int i = 0; i < 2; i++) {
  if (a.dword[i] == 0) {
    dst = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | N/A     | 2                |
| 3A6000   | LA664  | N/A     | 2                |
| 3C6000   | LA664  | N/A     | 2                |
| 2K1000LA | LA264  | N/A     | 1                |
| 2K3000   | LA364E | N/A     | 1                |
