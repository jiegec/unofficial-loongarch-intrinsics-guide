# x86 Logical

## x86and.b

### Synopsis

```
Instruction: x86and.b rj, rk
CPU Flags: LBT
```

### Description

x86-style bitwise AND: compute 8-bit AND of values in `rj` and `rk`. Clear CF, OF, AF. Update PF, ZF, SF according to the result. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint8_t lhs = (uint8_t)a;
uint8_t rhs = (uint8_t)b;
uint8_t result = lhs & rhs;
EFLAGS.CF = 0;
EFLAGS.OF = 0;
EFLAGS.AF = 0;
EFLAGS.PF = parity_even((uint8_t)result);
EFLAGS.ZF = result == 0;
EFLAGS.SF = (int8_t)result < 0;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86and.h

### Synopsis

```
Instruction: x86and.h rj, rk
CPU Flags: LBT
```

### Description

x86-style bitwise AND: compute 16-bit AND of values in `rj` and `rk`. Clear CF, OF, AF. Update PF, ZF, SF according to the result. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint16_t lhs = (uint16_t)a;
uint16_t rhs = (uint16_t)b;
uint16_t result = lhs & rhs;
EFLAGS.CF = 0;
EFLAGS.OF = 0;
EFLAGS.AF = 0;
EFLAGS.PF = parity_even((uint8_t)result);
EFLAGS.ZF = result == 0;
EFLAGS.SF = (int16_t)result < 0;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86and.w

### Synopsis

```
Instruction: x86and.w rj, rk
CPU Flags: LBT
```

### Description

x86-style bitwise AND: compute 32-bit AND of values in `rj` and `rk`. Clear CF, OF, AF. Update PF, ZF, SF according to the result. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint32_t lhs = (uint32_t)a;
uint32_t rhs = (uint32_t)b;
uint32_t result = lhs & rhs;
EFLAGS.CF = 0;
EFLAGS.OF = 0;
EFLAGS.AF = 0;
EFLAGS.PF = parity_even((uint8_t)result);
EFLAGS.ZF = result == 0;
EFLAGS.SF = (int32_t)result < 0;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86and.d

### Synopsis

```
Instruction: x86and.d rj, rk
CPU Flags: LBT
```

### Description

x86-style bitwise AND: compute 64-bit AND of values in `rj` and `rk`. Clear CF, OF, AF. Update PF, ZF, SF according to the result. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint64_t lhs = (uint64_t)a;
uint64_t rhs = (uint64_t)b;
uint64_t result = lhs & rhs;
EFLAGS.CF = 0;
EFLAGS.OF = 0;
EFLAGS.AF = 0;
EFLAGS.PF = parity_even((uint8_t)result);
EFLAGS.ZF = result == 0;
EFLAGS.SF = (int64_t)result < 0;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86or.b

### Synopsis

```
Instruction: x86or.b rj, rk
CPU Flags: LBT
```

### Description

x86-style bitwise OR: compute 8-bit OR of values in `rj` and `rk`. Clear CF, OF, AF. Update PF, ZF, SF according to the result. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint8_t lhs = (uint8_t)a;
uint8_t rhs = (uint8_t)b;
uint8_t result = lhs | rhs;
EFLAGS.CF = 0;
EFLAGS.OF = 0;
EFLAGS.AF = 0;
EFLAGS.PF = parity_even((uint8_t)result);
EFLAGS.ZF = result == 0;
EFLAGS.SF = (int8_t)result < 0;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86or.h

### Synopsis

```
Instruction: x86or.h rj, rk
CPU Flags: LBT
```

### Description

x86-style bitwise OR: compute 16-bit OR of values in `rj` and `rk`. Clear CF, OF, AF. Update PF, ZF, SF according to the result. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint16_t lhs = (uint16_t)a;
uint16_t rhs = (uint16_t)b;
uint16_t result = lhs | rhs;
EFLAGS.CF = 0;
EFLAGS.OF = 0;
EFLAGS.AF = 0;
EFLAGS.PF = parity_even((uint8_t)result);
EFLAGS.ZF = result == 0;
EFLAGS.SF = (int16_t)result < 0;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86or.w

### Synopsis

```
Instruction: x86or.w rj, rk
CPU Flags: LBT
```

### Description

x86-style bitwise OR: compute 32-bit OR of values in `rj` and `rk`. Clear CF, OF, AF. Update PF, ZF, SF according to the result. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint32_t lhs = (uint32_t)a;
uint32_t rhs = (uint32_t)b;
uint32_t result = lhs | rhs;
EFLAGS.CF = 0;
EFLAGS.OF = 0;
EFLAGS.AF = 0;
EFLAGS.PF = parity_even((uint8_t)result);
EFLAGS.ZF = result == 0;
EFLAGS.SF = (int32_t)result < 0;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86or.d

### Synopsis

```
Instruction: x86or.d rj, rk
CPU Flags: LBT
```

### Description

x86-style bitwise OR: compute 64-bit OR of values in `rj` and `rk`. Clear CF, OF, AF. Update PF, ZF, SF according to the result. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint64_t lhs = (uint64_t)a;
uint64_t rhs = (uint64_t)b;
uint64_t result = lhs | rhs;
EFLAGS.CF = 0;
EFLAGS.OF = 0;
EFLAGS.AF = 0;
EFLAGS.PF = parity_even((uint8_t)result);
EFLAGS.ZF = result == 0;
EFLAGS.SF = (int64_t)result < 0;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86xor.b

### Synopsis

```
Instruction: x86xor.b rj, rk
CPU Flags: LBT
```

### Description

x86-style bitwise XOR: compute 8-bit XOR of values in `rj` and `rk`. Clear CF, OF, AF. Update PF, ZF, SF according to the result. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint8_t lhs = (uint8_t)a;
uint8_t rhs = (uint8_t)b;
uint8_t result = lhs ^ rhs;
EFLAGS.CF = 0;
EFLAGS.OF = 0;
EFLAGS.AF = 0;
EFLAGS.PF = parity_even((uint8_t)result);
EFLAGS.ZF = result == 0;
EFLAGS.SF = (int8_t)result < 0;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86xor.h

### Synopsis

```
Instruction: x86xor.h rj, rk
CPU Flags: LBT
```

### Description

x86-style bitwise XOR: compute 16-bit XOR of values in `rj` and `rk`. Clear CF, OF, AF. Update PF, ZF, SF according to the result. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint16_t lhs = (uint16_t)a;
uint16_t rhs = (uint16_t)b;
uint16_t result = lhs ^ rhs;
EFLAGS.CF = 0;
EFLAGS.OF = 0;
EFLAGS.AF = 0;
EFLAGS.PF = parity_even((uint8_t)result);
EFLAGS.ZF = result == 0;
EFLAGS.SF = (int16_t)result < 0;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86xor.w

### Synopsis

```
Instruction: x86xor.w rj, rk
CPU Flags: LBT
```

### Description

x86-style bitwise XOR: compute 32-bit XOR of values in `rj` and `rk`. Clear CF, OF, AF. Update PF, ZF, SF according to the result. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint32_t lhs = (uint32_t)a;
uint32_t rhs = (uint32_t)b;
uint32_t result = lhs ^ rhs;
EFLAGS.CF = 0;
EFLAGS.OF = 0;
EFLAGS.AF = 0;
EFLAGS.PF = parity_even((uint8_t)result);
EFLAGS.ZF = result == 0;
EFLAGS.SF = (int32_t)result < 0;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86xor.d

### Synopsis

```
Instruction: x86xor.d rj, rk
CPU Flags: LBT
```

### Description

x86-style bitwise XOR: compute 64-bit XOR of values in `rj` and `rk`. Clear CF, OF, AF. Update PF, ZF, SF according to the result. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint64_t lhs = (uint64_t)a;
uint64_t rhs = (uint64_t)b;
uint64_t result = lhs ^ rhs;
EFLAGS.CF = 0;
EFLAGS.OF = 0;
EFLAGS.AF = 0;
EFLAGS.PF = parity_even((uint8_t)result);
EFLAGS.ZF = result == 0;
EFLAGS.SF = (int64_t)result < 0;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |
