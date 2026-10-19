# Branch

## int \_\_lasx_xbz_v (\_\_m256i a)

### Synopsis

```
int __lasx_xbz_v (__m256i a)
#include <lasxintrin.h>
Instruction: xvseteqz.v fcc, xr; bcnez
CPU Flags: LASX
```

### Description

Expected to be used in branches: branch if the whole vector `a` equals to zero.

### Operation

```
dst = a.qword[0] == 0 && a.qword[1] == 0;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | N/A     | 2                |
| 3A6000 | LA664 | N/A     | 2                |
| 3C6000 | LA664 | N/A     | 2                |

## int \_\_lasx_xbnz_v (\_\_m256i a)

### Synopsis

```
int __lasx_xbnz_v (__m256i a)
#include <lasxintrin.h>
Instruction: xvsetnez.v fcc, xr; bcnez
CPU Flags: LASX
```

### Description

Expected to be used in branches: branch if the whole vector `a` is non-zero.

### Operation

```
dst = a.qword[0] != 0 || a.qword[1] != 0;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | N/A     | 2                |
| 3A6000 | LA664 | N/A     | 2                |
| 3C6000 | LA664 | N/A     | 2                |

## int \_\_lasx_xbz_b (\_\_m256i a)

### Synopsis

```
int __lasx_xbz_b (__m256i a)
#include <lasxintrin.h>
Instruction: xvsetanyeqz.b fcc, xr; bcnez
CPU Flags: LASX
```

### Description

Expected to be used in branches: branch if any 8-bit element in `a` equals to zero.

### Operation

```
dst = 0;
for (int i = 0; i < 32; i++) {
  if (a.byte[i] == 0) {
    dst = 1;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | N/A     | 2                |
| 3A6000 | LA664 | N/A     | 2                |
| 3C6000 | LA664 | N/A     | 2                |

## int \_\_lasx_xbz_h (\_\_m256i a)

### Synopsis

```
int __lasx_xbz_h (__m256i a)
#include <lasxintrin.h>
Instruction: xvsetanyeqz.h fcc, xr; bcnez
CPU Flags: LASX
```

### Description

Expected to be used in branches: branch if any 16-bit element in `a` equals to zero.

### Operation

```
dst = 0;
for (int i = 0; i < 16; i++) {
  if (a.half[i] == 0) {
    dst = 1;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | N/A     | 2                |
| 3A6000 | LA664 | N/A     | 2                |
| 3C6000 | LA664 | N/A     | 2                |

## int \_\_lasx_xbz_w (\_\_m256i a)

### Synopsis

```
int __lasx_xbz_w (__m256i a)
#include <lasxintrin.h>
Instruction: xvsetanyeqz.w fcc, xr; bcnez
CPU Flags: LASX
```

### Description

Expected to be used in branches: branch if any 32-bit element in `a` equals to zero.

### Operation

```
dst = 0;
for (int i = 0; i < 8; i++) {
  if (a.word[i] == 0) {
    dst = 1;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | N/A     | 2                |
| 3A6000 | LA664 | N/A     | 2                |
| 3C6000 | LA664 | N/A     | 2                |

## int \_\_lasx_xbz_d (\_\_m256i a)

### Synopsis

```
int __lasx_xbz_d (__m256i a)
#include <lasxintrin.h>
Instruction: xvsetanyeqz.d fcc, xr; bcnez
CPU Flags: LASX
```

### Description

Expected to be used in branches: branch if any 64-bit element in `a` equals to zero.

### Operation

```
dst = 0;
for (int i = 0; i < 4; i++) {
  if (a.dword[i] == 0) {
    dst = 1;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | N/A     | 2                |
| 3A6000 | LA664 | N/A     | 2                |
| 3C6000 | LA664 | N/A     | 2                |

## int \_\_lasx_xbnz_b (\_\_m256i a)

### Synopsis

```
int __lasx_xbnz_b (__m256i a)
#include <lasxintrin.h>
Instruction: xvsetallnez.b fcc, xr; bcnez
CPU Flags: LASX
```

### Description

Expected to be used in branches: branch if all 8-bit elements in `a` are non-zero.

### Operation

```
dst = 1;
for (int i = 0; i < 32; i++) {
  if (a.byte[i] == 0) {
    dst = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | N/A     | 2                |
| 3A6000 | LA664 | N/A     | 2                |
| 3C6000 | LA664 | N/A     | 2                |

## int \_\_lasx_xbnz_h (\_\_m256i a)

### Synopsis

```
int __lasx_xbnz_h (__m256i a)
#include <lasxintrin.h>
Instruction: xvsetallnez.h fcc, xr; bcnez
CPU Flags: LASX
```

### Description

Expected to be used in branches: branch if all 16-bit elements in `a` are non-zero.

### Operation

```
dst = 1;
for (int i = 0; i < 16; i++) {
  if (a.half[i] == 0) {
    dst = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | N/A     | 2                |
| 3A6000 | LA664 | N/A     | 2                |
| 3C6000 | LA664 | N/A     | 2                |

## int \_\_lasx_xbnz_w (\_\_m256i a)

### Synopsis

```
int __lasx_xbnz_w (__m256i a)
#include <lasxintrin.h>
Instruction: xvsetallnez.w fcc, xr; bcnez
CPU Flags: LASX
```

### Description

Expected to be used in branches: branch if all 32-bit elements in `a` are non-zero.

### Operation

```
dst = 1;
for (int i = 0; i < 8; i++) {
  if (a.word[i] == 0) {
    dst = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | N/A     | 2                |
| 3A6000 | LA664 | N/A     | 2                |
| 3C6000 | LA664 | N/A     | 2                |

## int \_\_lasx_xbnz_d (\_\_m256i a)

### Synopsis

```
int __lasx_xbnz_d (__m256i a)
#include <lasxintrin.h>
Instruction: xvsetallnez.d fcc, xr; bcnez
CPU Flags: LASX
```

### Description

Expected to be used in branches: branch if all 64-bit elements in `a` are non-zero.

### Operation

```
dst = 1;
for (int i = 0; i < 4; i++) {
  if (a.dword[i] == 0) {
    dst = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | N/A     | 2                |
| 3A6000 | LA664 | N/A     | 2                |
| 3C6000 | LA664 | N/A     | 2                |
