# ARM Shift

## armsll.w

### Synopsis

```
Instruction: armsll.w rj, rk, cond
CPU Flags: LBT
```

### Description

ARM-style logical shift left (32-bit): conditionally shift value in `rj` left by amount in `rk` (masked to 8 bits). Update ARMFLAGS (C, N, Z). Only ARMFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t lhs = (uint32_t)a;
  uint32_t c = (uint32_t)b & 0xff;
  uint32_t r;
  if (c == 0) {
    r = lhs;
  } else if (c < 32) {
    ARMFLAGS.C = (lhs >> (32 - c)) & 1;
    r = lhs << c;
  } else if (c == 32) {
    ARMFLAGS.C = lhs & 1;
    r = 0;
  } else {
    ARMFLAGS.C = 0;
    r = 0;
  }
  ARMFLAGS.N = (r >> 31) & 1;
  ARMFLAGS.Z = r == 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## armsrl.w

### Synopsis

```
Instruction: armsrl.w rj, rk, cond
CPU Flags: LBT
```

### Description

ARM-style logical shift right (32-bit): conditionally shift value in `rj` right logically by amount in `rk` (masked to 8 bits). Update ARMFLAGS (C, N, Z). Only ARMFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t lhs = (uint32_t)a;
  uint32_t rhs = (uint32_t)b;
  uint32_t r;
  uint32_t c = rhs & 0xff;
  if (c == 0) {
    r = lhs;
  } else if (c < 32) {
    r = lhs >> c;
    ARMFLAGS.C = (lhs >> (c - 1)) & 1;
  } else if (c == 32) {
    r = 0;
    ARMFLAGS.C = (lhs >> 31) & 1;
  } else {
    r = 0;
    ARMFLAGS.C = 0;
  }
  ARMFLAGS.N = (r >> 31) & 1;
  ARMFLAGS.Z = r == 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## armsra.w

### Synopsis

```
Instruction: armsra.w rj, rk, cond
CPU Flags: LBT
```

### Description

ARM-style arithmetic shift right (32-bit): conditionally shift value in `rj` right arithmetically by amount in `rk` (masked to 8 bits). Update ARMFLAGS (C, N, Z). Only ARMFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t lhs = (uint32_t)a;
  uint32_t rhs = (uint32_t)b;
  uint32_t r;
  uint32_t c = rhs & 0xff;
  if (c == 0) {
    r = lhs;
  } else if (c < 32) {
    r = (uint32_t)((int32_t)lhs >> c);
    ARMFLAGS.C = (lhs >> (c - 1)) & 1;
  } else {
    ARMFLAGS.C = (lhs >> 31) & 1;
    r = ARMFLAGS.C ? UINT32_MAX : 0;
  }
  ARMFLAGS.N = (r >> 31) & 1;
  ARMFLAGS.Z = r == 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## armrotr.w

### Synopsis

```
Instruction: armrotr.w rj, rk, cond
CPU Flags: LBT
```

### Description

ARM-style rotate right (32-bit): conditionally rotate value in `rj` right by amount in `rk` (masked to 8 bits, rotate modulo 32). Update ARMFLAGS (C, N, Z). Only ARMFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t lhs = (uint32_t)a;
  uint32_t rhs = (uint32_t)b;
  uint32_t raw_count = rhs & 0xff;
  uint32_t c = rhs & 0x1f;
  unsigned n = c % 32;
  uint32_t r = n == 0 ? lhs : (lhs >> n) | (lhs << (32 - n));
  if (raw_count != 0) {
    ARMFLAGS.C = n == 0 ? ((lhs >> 31) & 1) : ((lhs >> (n - 1)) & 1);
  }
  ARMFLAGS.N = (r >> 31) & 1;
  ARMFLAGS.Z = r == 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## armrrx.w

### Synopsis

```
Instruction: armrrx.w rj, cond
CPU Flags: LBT
```

### Description

ARM-style rotate right extended (32-bit): conditionally rotate the value in `rj` and C (in ARMFLAGS) together as a 33-bit ring right by 1. Update ARMFLAGS (C, N, Z). Only ARMFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t r = ((uint32_t)a >> 1) | (ARMFLAGS.C ? 0x80000000 : 0);
  ARMFLAGS.C = a & 1;
  ARMFLAGS.N = (r >> 31) & 1;
  ARMFLAGS.Z = r == 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## armslli.w

### Synopsis

```
Instruction: armslli.w rj, imm, cond
CPU Flags: LBT
```

### Description

ARM-style logical shift left immediate (32-bit): conditionally shift value in `rj` left by `imm`. Update ARMFLAGS (C, N, Z). Only ARMFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t v = (uint32_t)a;
  uint32_t r = v << imm;
  ARMFLAGS.C = imm == 0 ? ARMFLAGS.C : (imm > 32 ? 0 : ((v >> (32 - imm)) & 1));
  ARMFLAGS.N = (r >> 31) & 1;
  ARMFLAGS.Z = r == 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## armsrli.w

### Synopsis

```
Instruction: armsrli.w rj, imm, cond
CPU Flags: LBT
```

### Description

ARM-style logical shift right immediate (32-bit): conditionally shift value in `rj` right logically by `imm`. Update ARMFLAGS (C, N, Z). Only ARMFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t v = (uint32_t)a;
  uint32_t r = v >> imm;
  ARMFLAGS.C = imm == 0 ? ARMFLAGS.C : (imm > 32 ? 0 : ((v >> (imm - 1)) & 1));
  ARMFLAGS.N = (r >> 31) & 1;
  ARMFLAGS.Z = r == 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## armsrai.w

### Synopsis

```
Instruction: armsrai.w rj, imm, cond
CPU Flags: LBT
```

### Description

ARM-style arithmetic shift right immediate (32-bit): conditionally shift value in `rj` right arithmetically by `imm`. Update ARMFLAGS (C, N, Z). Only ARMFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t v = (uint32_t)a;
  uint32_t r = (uint32_t)((int32_t)v >> imm);
  ARMFLAGS.C = imm == 0 ? ARMFLAGS.C : (imm > 32 ? 0 : ((v >> (imm - 1)) & 1));
  ARMFLAGS.N = (r >> 31) & 1;
  ARMFLAGS.Z = r == 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## armrotri.w

### Synopsis

```
Instruction: armrotri.w rj, imm, cond
CPU Flags: LBT
```

### Description

ARM-style rotate right immediate (32-bit): conditionally rotate value in `rj` right by `imm`. Update ARMFLAGS (C, N, Z). Only ARMFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t v = (uint32_t)a;
  uint32_t r = imm == 0 ? v : (v >> imm) | (v << (32 - imm));
  if (imm != 0) {
    ARMFLAGS.C = (v >> (imm - 1)) & 1;
  }
  ARMFLAGS.N = (r >> 31) & 1;
  ARMFLAGS.Z = r == 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |
