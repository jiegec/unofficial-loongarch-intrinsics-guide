# ARM Integer

## armadd.w

### Synopsis

```
Instruction: armadd.w rj, rk, cond
CPU Flags: LBT
```

### Description

ARM-style add (32-bit): conditionally add values in `rj` and `rk`. Update ARMFLAGS (C, V, N, Z). Only ARMFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t lhs = (uint32_t)a;
  uint32_t rhs = (uint32_t)b;
  uint64_t wide = (uint64_t)lhs + rhs;
  uint32_t r = (uint32_t)wide;
  ARMFLAGS.C = (wide >> 32) & 1;
  ARMFLAGS.V = ((~(lhs ^ rhs)) & (lhs ^ r) & 0x80000000) != 0;
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

## armsub.w

### Synopsis

```
Instruction: armsub.w rj, rk, cond
CPU Flags: LBT
```

### Description

ARM-style subtract (32-bit): conditionally subtract value in `rk` from `rj`. Update ARMFLAGS (C, V, N, Z). Only ARMFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t lhs = (uint32_t)a;
  uint32_t rhs = (uint32_t)b;
  uint64_t subtrahend = (uint64_t)rhs;
  uint32_t r = (uint32_t)(lhs - subtrahend);
  ARMFLAGS.C = lhs < subtrahend;
  ARMFLAGS.V = ((lhs ^ rhs) & (lhs ^ r) & 0x80000000) != 0;
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

## armadc.w

### Synopsis

```
Instruction: armadc.w rj, rk, cond
CPU Flags: LBT
```

### Description

ARM-style add with carry (32-bit): conditionally add values in `rj` and `rk` with carry (C in ARMFLAGS). Update ARMFLAGS (C, V, N, Z). Only ARMFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t lhs = (uint32_t)a;
  uint32_t rhs = (uint32_t)b;
  uint32_t carry_in = ARMFLAGS.C;
  uint64_t wide = (uint64_t)lhs + rhs + carry_in;
  uint32_t r = (uint32_t)wide;
  ARMFLAGS.C = (wide >> 32) & 1;
  ARMFLAGS.V = ((~(lhs ^ rhs)) & (lhs ^ r) & 0x80000000) != 0;
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

## armsbc.w

### Synopsis

```
Instruction: armsbc.w rj, rk, cond
CPU Flags: LBT
```

### Description

ARM-style subtract with borrow (32-bit): conditionally subtract value in `rk` from `rj` with borrow (C in ARMFLAGS). Update ARMFLAGS (C, V, N, Z). Only ARMFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t lhs = (uint32_t)a;
  uint32_t rhs = (uint32_t)b;
  uint32_t borrow_in = ARMFLAGS.C ? 0 : 1;
  uint64_t subtrahend = (uint64_t)rhs + borrow_in;
  uint32_t r = (uint32_t)(lhs - subtrahend);
  ARMFLAGS.C = lhs < subtrahend;
  ARMFLAGS.V = ((lhs ^ (uint32_t)subtrahend) & (lhs ^ r) & 0x80000000) != 0;
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
