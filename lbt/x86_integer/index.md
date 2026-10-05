# x86 Integer

## x86adc.b

### Synopsis

```
Instruction: x86adc.b rj, rk
CPU Flags: LBT
```

### Description

x86-style add with carry: add 8-bit values in `rj` and `rk` with CF (in EFLAGS). Update EFLAGS (CF, AF, OF, PF, ZF, SF) according to the result. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint8_t lhs = (uint8_t)a;
uint8_t rhs = (uint8_t)b;
uint8_t carry_in = EFLAGS.CF;
uint8_t result = lhs + rhs + carry_in;
EFLAGS.CF = (uint64_t)lhs + rhs + carry_in > UINT8_MAX;
EFLAGS.AF = ((lhs ^ rhs ^ result) & 0x10) != 0;
EFLAGS.OF = ((~(lhs ^ rhs)) & (lhs ^ result) & 0x80) != 0;
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

## x86adc.h

### Synopsis

```
Instruction: x86adc.h rj, rk
CPU Flags: LBT
```

### Description

x86-style add with carry: add 16-bit values in `rj` and `rk` with CF (in EFLAGS). Update EFLAGS (CF, AF, OF, PF, ZF, SF) according to the result. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint16_t lhs = (uint16_t)a;
uint16_t rhs = (uint16_t)b;
uint8_t carry_in = EFLAGS.CF;
uint16_t result = lhs + rhs + carry_in;
EFLAGS.CF = (uint64_t)lhs + rhs + carry_in > UINT16_MAX;
EFLAGS.AF = ((lhs ^ rhs ^ result) & 0x10) != 0;
EFLAGS.OF = ((~(lhs ^ rhs)) & (lhs ^ result) & 0x8000) != 0;
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

## x86adc.w

### Synopsis

```
Instruction: x86adc.w rj, rk
CPU Flags: LBT
```

### Description

x86-style add with carry: add 32-bit values in `rj` and `rk` with CF (in EFLAGS). Update EFLAGS (CF, AF, OF, PF, ZF, SF) according to the result. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint32_t lhs = (uint32_t)a;
uint32_t rhs = (uint32_t)b;
uint8_t carry_in = EFLAGS.CF;
uint32_t result = lhs + rhs + carry_in;
EFLAGS.CF = (uint64_t)lhs + rhs + carry_in > UINT32_MAX;
EFLAGS.AF = ((lhs ^ rhs ^ result) & 0x10) != 0;
EFLAGS.OF = ((~(lhs ^ rhs)) & (lhs ^ result) & 0x80000000) != 0;
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

## x86adc.d

### Synopsis

```
Instruction: x86adc.d rj, rk
CPU Flags: LBT
```

### Description

x86-style add with carry: add 64-bit values in `rj` and `rk` with CF (in EFLAGS). Update EFLAGS (CF, AF, OF, PF, ZF, SF) according to the result. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint64_t lhs = (uint64_t)a;
uint64_t rhs = (uint64_t)b;
uint8_t carry_in = EFLAGS.CF;
uint64_t result = lhs + rhs + carry_in;
EFLAGS.CF = lhs > UINT64_MAX - rhs - carry_in;
EFLAGS.AF = ((lhs ^ rhs ^ result) & 0x10) != 0;
EFLAGS.OF = ((~(lhs ^ rhs)) & (lhs ^ result) & 0x8000000000000000ULL) != 0;
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

## x86add.b

### Synopsis

```
Instruction: x86add.b rj, rk
CPU Flags: LBT
```

### Description

x86-style add: add 8-bit values in `rj` and `rk`. Update EFLAGS (CF, AF, OF, PF, ZF, SF) according to the result. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint8_t lhs = (uint8_t)a;
uint8_t rhs = (uint8_t)b;
uint8_t result = lhs + rhs;
EFLAGS.CF = (uint64_t)lhs + (uint64_t)rhs > UINT8_MAX;
EFLAGS.AF = ((lhs ^ rhs ^ result) & 0x10) != 0;
EFLAGS.OF = ((~(lhs ^ rhs)) & (lhs ^ result) & 0x80) != 0;
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

## x86add.h

### Synopsis

```
Instruction: x86add.h rj, rk
CPU Flags: LBT
```

### Description

x86-style add: add 16-bit values in `rj` and `rk`. Update EFLAGS (CF, AF, OF, PF, ZF, SF) according to the result. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint16_t lhs = (uint16_t)a;
uint16_t rhs = (uint16_t)b;
uint16_t result = lhs + rhs;
EFLAGS.CF = (uint64_t)lhs + (uint64_t)rhs > UINT16_MAX;
EFLAGS.AF = ((lhs ^ rhs ^ result) & 0x10) != 0;
EFLAGS.OF = ((~(lhs ^ rhs)) & (lhs ^ result) & 0x8000) != 0;
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

## x86add.w

### Synopsis

```
Instruction: x86add.w rj, rk
CPU Flags: LBT
```

### Description

x86-style add: add 32-bit values in `rj` and `rk`. Update EFLAGS (CF, AF, OF, PF, ZF, SF) according to the result. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint32_t lhs = (uint32_t)a;
uint32_t rhs = (uint32_t)b;
uint32_t result = lhs + rhs;
EFLAGS.CF = (uint64_t)lhs + (uint64_t)rhs > UINT32_MAX;
EFLAGS.AF = ((lhs ^ rhs ^ result) & 0x10) != 0;
EFLAGS.OF = ((~(lhs ^ rhs)) & (lhs ^ result) & 0x80000000) != 0;
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

## x86add.d

### Synopsis

```
Instruction: x86add.d rj, rk
CPU Flags: LBT
```

### Description

x86-style add: add 64-bit values in `rj` and `rk`. Update EFLAGS (CF, AF, OF, PF, ZF, SF) according to the result. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint64_t lhs = (uint64_t)a;
uint64_t rhs = (uint64_t)b;
uint64_t result = lhs + rhs;
EFLAGS.CF = lhs > UINT64_MAX - rhs;
EFLAGS.AF = ((lhs ^ rhs ^ result) & 0x10) != 0;
EFLAGS.OF = ((~(lhs ^ rhs)) & (lhs ^ result) & 0x8000000000000000ULL) != 0;
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

## x86add.wu

### Synopsis

```
Instruction: x86add.wu rj, rk
CPU Flags: LBT
```

### Description

x86-style add: add unsigned 32-bit values in `rj` and `rk`. Update EFLAGS (CF, AF, OF, PF, ZF, SF) according to the result. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint32_t lhs = (uint32_t)a;
uint32_t rhs = (uint32_t)b;
uint32_t result = lhs + rhs;
EFLAGS.CF = (uint64_t)lhs + (uint64_t)rhs > UINT32_MAX;
EFLAGS.AF = ((lhs ^ rhs ^ result) & 0x10) != 0;
EFLAGS.OF = 0;
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

## x86add.du

### Synopsis

```
Instruction: x86add.du rj, rk
CPU Flags: LBT
```

### Description

x86-style add: add unsigned 64-bit values in `rj` and `rk`. Update EFLAGS (CF, AF, OF, PF, ZF, SF) according to the result. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint64_t lhs = (uint64_t)a;
uint64_t rhs = (uint64_t)b;
uint64_t result = lhs + rhs;
EFLAGS.CF = lhs > UINT64_MAX - rhs;
EFLAGS.AF = ((lhs ^ rhs ^ result) & 0x10) != 0;
EFLAGS.OF = 0;
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

## x86inc.b

### Synopsis

```
Instruction: x86inc.b rj
CPU Flags: LBT
```

### Description

x86-style increment: add 1 to the 8-bit value in `rj`. Update EFLAGS (AF, OF, PF, ZF, SF). Preserve CF. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint8_t v = (uint8_t)a;
uint8_t r = v + 1;
// CF preserved from input EFLAGS.CF
EFLAGS.PF = parity_even((uint8_t)r);
EFLAGS.AF = ((v ^ 1 ^ r) & 0x10) != 0;
EFLAGS.ZF = r == 0;
EFLAGS.SF = (int8_t)r < 0;
EFLAGS.OF = v == (uint8_t)INT8_MAX;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86inc.h

### Synopsis

```
Instruction: x86inc.h rj
CPU Flags: LBT
```

### Description

x86-style increment: add 1 to the 16-bit value in `rj`. Update EFLAGS (AF, OF, PF, ZF, SF). Preserve CF. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint16_t v = (uint16_t)a;
uint16_t r = v + 1;
// CF preserved from input EFLAGS.CF
EFLAGS.PF = parity_even((uint8_t)r);
EFLAGS.AF = ((v ^ 1 ^ r) & 0x10) != 0;
EFLAGS.ZF = r == 0;
EFLAGS.SF = (int16_t)r < 0;
EFLAGS.OF = v == (uint16_t)INT16_MAX;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86inc.w

### Synopsis

```
Instruction: x86inc.w rj
CPU Flags: LBT
```

### Description

x86-style increment: add 1 to the 32-bit value in `rj`. Update EFLAGS (AF, OF, PF, ZF, SF). Preserve CF. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint32_t v = (uint32_t)a;
uint32_t r = v + 1;
// CF preserved from input EFLAGS.CF
EFLAGS.PF = parity_even((uint8_t)r);
EFLAGS.AF = ((v ^ 1 ^ r) & 0x10) != 0;
EFLAGS.ZF = r == 0;
EFLAGS.SF = (int32_t)r < 0;
EFLAGS.OF = v == (uint32_t)INT32_MAX;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86inc.d

### Synopsis

```
Instruction: x86inc.d rj
CPU Flags: LBT
```

### Description

x86-style increment: add 1 to the 64-bit value in `rj`. Update EFLAGS (AF, OF, PF, ZF, SF). Preserve CF. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint64_t v = (uint64_t)a;
uint64_t r = v + 1;
// CF preserved from input EFLAGS.CF
EFLAGS.PF = parity_even((uint8_t)r);
EFLAGS.AF = ((v ^ 1 ^ r) & 0x10) != 0;
EFLAGS.ZF = r == 0;
EFLAGS.SF = (int64_t)r < 0;
EFLAGS.OF = v == (uint64_t)INT64_MAX;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86sbc.b

### Synopsis

```
Instruction: x86sbc.b rj, rk
CPU Flags: LBT
```

### Description

x86-style subtract with borrow: subtract 8-bit values in `rj` and `rk` with CF (in EFLAGS). Update EFLAGS (CF, AF, OF, PF, ZF, SF) according to the result. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint8_t lhs = (uint8_t)a;
uint8_t rhs = (uint8_t)b;
uint8_t carry_in = EFLAGS.CF;
uint64_t subtrahend = (uint64_t)rhs + carry_in;
uint8_t result = (uint8_t)(lhs - subtrahend);
EFLAGS.CF = lhs < subtrahend;
EFLAGS.AF = (lhs & 0xf) < (rhs & 0xf);
EFLAGS.OF = ((lhs ^ rhs) & (lhs ^ result) & 0x80) != 0;
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

## x86sbc.h

### Synopsis

```
Instruction: x86sbc.h rj, rk
CPU Flags: LBT
```

### Description

x86-style subtract with borrow: subtract 16-bit values in `rj` and `rk` with CF (in EFLAGS). Update EFLAGS (CF, AF, OF, PF, ZF, SF) according to the result. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint16_t lhs = (uint16_t)a;
uint16_t rhs = (uint16_t)b;
uint8_t carry_in = EFLAGS.CF;
uint64_t subtrahend = (uint64_t)rhs + carry_in;
uint16_t result = (uint16_t)(lhs - subtrahend);
EFLAGS.CF = lhs < subtrahend;
EFLAGS.AF = (lhs & 0xf) < (rhs & 0xf);
EFLAGS.OF = ((lhs ^ rhs) & (lhs ^ result) & 0x8000) != 0;
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

## x86sbc.w

### Synopsis

```
Instruction: x86sbc.w rj, rk
CPU Flags: LBT
```

### Description

x86-style subtract with borrow: subtract 32-bit values in `rj` and `rk` with CF (in EFLAGS). Update EFLAGS (CF, AF, OF, PF, ZF, SF) according to the result. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint32_t lhs = (uint32_t)a;
uint32_t rhs = (uint32_t)b;
uint8_t carry_in = EFLAGS.CF;
uint64_t subtrahend = (uint64_t)rhs + carry_in;
uint32_t result = (uint32_t)(lhs - subtrahend);
EFLAGS.CF = lhs < subtrahend;
EFLAGS.AF = (lhs & 0xf) < (rhs & 0xf);
EFLAGS.OF = ((lhs ^ rhs) & (lhs ^ result) & 0x80000000) != 0;
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

## x86sbc.d

### Synopsis

```
Instruction: x86sbc.d rj, rk
CPU Flags: LBT
```

### Description

x86-style subtract with borrow: subtract 64-bit values in `rj` and `rk` with CF (in EFLAGS). Update EFLAGS (CF, AF, OF, PF, ZF, SF) according to the result. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint64_t lhs = (uint64_t)a;
uint64_t rhs = (uint64_t)b;
uint8_t carry_in = EFLAGS.CF;
uint64_t subtrahend = (uint64_t)rhs + carry_in;
uint64_t result = (uint64_t)(lhs - subtrahend);
EFLAGS.CF = lhs < subtrahend;
EFLAGS.AF = (lhs & 0xf) < (rhs & 0xf);
EFLAGS.OF = ((lhs ^ rhs) & (lhs ^ result) & 0x8000000000000000ULL) != 0;
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

## x86sub.b

### Synopsis

```
Instruction: x86sub.b rj, rk
CPU Flags: LBT
```

### Description

x86-style subtract: subtract 8-bit values in `rj` and `rk`. Update EFLAGS (CF, AF, OF, PF, ZF, SF) according to the result. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint8_t lhs = (uint8_t)a;
uint8_t rhs = (uint8_t)b;
uint8_t result = lhs - rhs;
EFLAGS.CF = lhs < rhs;
EFLAGS.AF = ((lhs ^ rhs ^ result) & 0x10) != 0;
EFLAGS.OF = ((lhs ^ rhs) & (lhs ^ result) & 0x80) != 0;
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

## x86sub.h

### Synopsis

```
Instruction: x86sub.h rj, rk
CPU Flags: LBT
```

### Description

x86-style subtract: subtract 16-bit values in `rj` and `rk`. Update EFLAGS (CF, AF, OF, PF, ZF, SF) according to the result. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint16_t lhs = (uint16_t)a;
uint16_t rhs = (uint16_t)b;
uint16_t result = lhs - rhs;
EFLAGS.CF = lhs < rhs;
EFLAGS.AF = ((lhs ^ rhs ^ result) & 0x10) != 0;
EFLAGS.OF = ((lhs ^ rhs) & (lhs ^ result) & 0x8000) != 0;
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

## x86sub.w

### Synopsis

```
Instruction: x86sub.w rj, rk
CPU Flags: LBT
```

### Description

x86-style subtract: subtract 32-bit values in `rj` and `rk`. Update EFLAGS (CF, AF, OF, PF, ZF, SF) according to the result. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint32_t lhs = (uint32_t)a;
uint32_t rhs = (uint32_t)b;
uint32_t result = lhs - rhs;
EFLAGS.CF = lhs < rhs;
EFLAGS.AF = ((lhs ^ rhs ^ result) & 0x10) != 0;
EFLAGS.OF = ((lhs ^ rhs) & (lhs ^ result) & 0x80000000) != 0;
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

## x86sub.d

### Synopsis

```
Instruction: x86sub.d rj, rk
CPU Flags: LBT
```

### Description

x86-style subtract: subtract 64-bit values in `rj` and `rk`. Update EFLAGS (CF, AF, OF, PF, ZF, SF) according to the result. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint64_t lhs = (uint64_t)a;
uint64_t rhs = (uint64_t)b;
uint64_t result = lhs - rhs;
EFLAGS.CF = lhs < rhs;
EFLAGS.AF = ((lhs ^ rhs ^ result) & 0x10) != 0;
EFLAGS.OF = ((lhs ^ rhs) & (lhs ^ result) & 0x8000000000000000ULL) != 0;
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

## x86sub.wu

### Synopsis

```
Instruction: x86sub.wu rj, rk
CPU Flags: LBT
```

### Description

x86-style subtract: subtract unsigned 32-bit values in `rj` and `rk`. Update EFLAGS (CF, AF, OF, PF, ZF, SF) according to the result. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint32_t lhs = (uint32_t)a;
uint32_t rhs = (uint32_t)b;
uint32_t result = lhs - rhs;
EFLAGS.CF = lhs < rhs;
EFLAGS.AF = ((lhs ^ rhs ^ result) & 0x10) != 0;
EFLAGS.OF = 0;
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

## x86sub.du

### Synopsis

```
Instruction: x86sub.du rj, rk
CPU Flags: LBT
```

### Description

x86-style subtract: subtract unsigned 64-bit values in `rj` and `rk`. Update EFLAGS (CF, AF, OF, PF, ZF, SF) according to the result. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint64_t lhs = (uint64_t)a;
uint64_t rhs = (uint64_t)b;
uint64_t result = lhs - rhs;
EFLAGS.CF = lhs < rhs;
EFLAGS.AF = ((lhs ^ rhs ^ result) & 0x10) != 0;
EFLAGS.OF = 0;
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

## x86dec.b

### Synopsis

```
Instruction: x86dec.b rj
CPU Flags: LBT
```

### Description

x86-style decrement: subtract 1 from the 8-bit value in `rj`. Update EFLAGS (AF, OF, PF, ZF, SF). Preserve CF. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint8_t v = (uint8_t)a;
uint8_t r = v - 1;
// CF preserved from input EFLAGS.CF
EFLAGS.PF = parity_even((uint8_t)r);
EFLAGS.AF = ((v ^ 1 ^ r) & 0x10) != 0;
EFLAGS.ZF = r == 0;
EFLAGS.SF = (int8_t)r < 0;
EFLAGS.OF = v == (uint8_t)INT8_MIN;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86dec.h

### Synopsis

```
Instruction: x86dec.h rj
CPU Flags: LBT
```

### Description

x86-style decrement: subtract 1 from the 16-bit value in `rj`. Update EFLAGS (AF, OF, PF, ZF, SF). Preserve CF. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint16_t v = (uint16_t)a;
uint16_t r = v - 1;
// CF preserved from input EFLAGS.CF
EFLAGS.PF = parity_even((uint8_t)r);
EFLAGS.AF = ((v ^ 1 ^ r) & 0x10) != 0;
EFLAGS.ZF = r == 0;
EFLAGS.SF = (int16_t)r < 0;
EFLAGS.OF = v == (uint16_t)INT16_MIN;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86dec.w

### Synopsis

```
Instruction: x86dec.w rj
CPU Flags: LBT
```

### Description

x86-style decrement: subtract 1 from the 32-bit value in `rj`. Update EFLAGS (AF, OF, PF, ZF, SF). Preserve CF. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint32_t v = (uint32_t)a;
uint32_t r = v - 1;
// CF preserved from input EFLAGS.CF
EFLAGS.PF = parity_even((uint8_t)r);
EFLAGS.AF = ((v ^ 1 ^ r) & 0x10) != 0;
EFLAGS.ZF = r == 0;
EFLAGS.SF = (int32_t)r < 0;
EFLAGS.OF = v == (uint32_t)INT32_MIN;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86dec.d

### Synopsis

```
Instruction: x86dec.d rj
CPU Flags: LBT
```

### Description

x86-style decrement: subtract 1 from the 64-bit value in `rj`. Update EFLAGS (AF, OF, PF, ZF, SF). Preserve CF. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint64_t v = (uint64_t)a;
uint64_t r = v - 1;
// CF preserved from input EFLAGS.CF
EFLAGS.PF = parity_even((uint8_t)r);
EFLAGS.AF = ((v ^ 1 ^ r) & 0x10) != 0;
EFLAGS.ZF = r == 0;
EFLAGS.SF = (int64_t)r < 0;
EFLAGS.OF = v == (uint64_t)INT64_MIN;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86mul.b

### Synopsis

```
Instruction: x86mul.b rj, rk
CPU Flags: LBT
```

### Description

x86-style multiply: multiply signed 8-bit values in `rj` and `rk`. Set CF and OF if overflow (result does not fit in 8-bits), clear other flags. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint8_t lhs = (uint8_t)a;
uint8_t rhs = (uint8_t)b;
__int128 product = (__int128)(int8_t)lhs * (__int128)(int8_t)rhs;
bool overflow = product < (__int128)INT8_MIN || product > (__int128)INT8_MAX;
EFLAGS.CF = overflow;
EFLAGS.OF = overflow;
EFLAGS.SF = 0;
EFLAGS.ZF = 0;
EFLAGS.AF = 0;
EFLAGS.PF = 0;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1.17    | 0.86(1/1.17)     |
| 3C6000 | LA664 | 1       | 1                |

## x86mul.h

### Synopsis

```
Instruction: x86mul.h rj, rk
CPU Flags: LBT
```

### Description

x86-style multiply: multiply signed 16-bit values in `rj` and `rk`. Set CF and OF if overflow (result does not fit in 16-bits), clear other flags. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint16_t lhs = (uint16_t)a;
uint16_t rhs = (uint16_t)b;
__int128 product = (__int128)(int16_t)lhs * (__int128)(int16_t)rhs;
bool overflow = product < (__int128)INT16_MIN || product > (__int128)INT16_MAX;
EFLAGS.CF = overflow;
EFLAGS.OF = overflow;
EFLAGS.SF = 0;
EFLAGS.ZF = 0;
EFLAGS.AF = 0;
EFLAGS.PF = 0;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1.17    | 0.86(1/1.17)     |
| 3C6000 | LA664 | 1       | 1                |

## x86mul.w

### Synopsis

```
Instruction: x86mul.w rj, rk
CPU Flags: LBT
```

### Description

x86-style multiply: multiply signed 32-bit values in `rj` and `rk`. Set CF and OF if overflow (result does not fit in 32-bits), clear other flags. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint32_t lhs = (uint32_t)a;
uint32_t rhs = (uint32_t)b;
__int128 product = (__int128)(int32_t)lhs * (__int128)(int32_t)rhs;
bool overflow = product < (__int128)INT32_MIN || product > (__int128)INT32_MAX;
EFLAGS.CF = overflow;
EFLAGS.OF = overflow;
EFLAGS.SF = 0;
EFLAGS.ZF = 0;
EFLAGS.AF = 0;
EFLAGS.PF = 0;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1.17    | 0.86(1/1.17)     |
| 3C6000 | LA664 | 1       | 1                |

## x86mul.d

### Synopsis

```
Instruction: x86mul.d rj, rk
CPU Flags: LBT
```

### Description

x86-style multiply: multiply signed 64-bit values in `rj` and `rk`. Set CF and OF if overflow (result does not fit in 64-bits), clear other flags. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint64_t lhs = (uint64_t)a;
uint64_t rhs = (uint64_t)b;
__int128 product = (__int128)(int64_t)lhs * (__int128)(int64_t)rhs;
bool overflow = product < (__int128)INT64_MIN || product > (__int128)INT64_MAX;
EFLAGS.CF = overflow;
EFLAGS.OF = overflow;
EFLAGS.SF = 0;
EFLAGS.ZF = 0;
EFLAGS.AF = 0;
EFLAGS.PF = 0;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1.17    | 0.86(1/1.17)     |
| 3C6000 | LA664 | 1       | 1                |

## x86mul.bu

### Synopsis

```
Instruction: x86mul.bu rj, rk
CPU Flags: LBT
```

### Description

x86-style multiply: multiply unsigned 8-bit values in `rj` and `rk`. Set CF and OF if overflow (result does not fit in 8-bits), clear other flags. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint8_t lhs = (uint8_t)a;
uint8_t rhs = (uint8_t)b;
unsigned __int128 product =
    (unsigned __int128)(uint8_t)lhs * (unsigned __int128)(uint8_t)rhs;
bool overflow = (product >> 8) != 0;
EFLAGS.CF = overflow;
EFLAGS.OF = overflow;
EFLAGS.SF = 0;
EFLAGS.ZF = 0;
EFLAGS.AF = 0;
EFLAGS.PF = 0;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1.17    | 0.86(1/1.17)     |
| 3C6000 | LA664 | 1       | 1                |

## x86mul.hu

### Synopsis

```
Instruction: x86mul.hu rj, rk
CPU Flags: LBT
```

### Description

x86-style multiply: multiply unsigned 16-bit values in `rj` and `rk`. Set CF and OF if overflow (result does not fit in 16-bits), clear other flags. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint16_t lhs = (uint16_t)a;
uint16_t rhs = (uint16_t)b;
unsigned __int128 product =
    (unsigned __int128)(uint16_t)lhs * (unsigned __int128)(uint16_t)rhs;
bool overflow = (product >> 16) != 0;
EFLAGS.CF = overflow;
EFLAGS.OF = overflow;
EFLAGS.SF = 0;
EFLAGS.ZF = 0;
EFLAGS.AF = 0;
EFLAGS.PF = 0;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1.17    | 0.86(1/1.17)     |
| 3C6000 | LA664 | 1       | 1                |

## x86mul.wu

### Synopsis

```
Instruction: x86mul.wu rj, rk
CPU Flags: LBT
```

### Description

x86-style multiply: multiply unsigned 32-bit values in `rj` and `rk`. Set CF and OF if overflow (result does not fit in 32-bits), clear other flags. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint32_t lhs = (uint32_t)a;
uint32_t rhs = (uint32_t)b;
unsigned __int128 product =
    (unsigned __int128)(uint32_t)lhs * (unsigned __int128)(uint32_t)rhs;
bool overflow = (product >> 32) != 0;
EFLAGS.CF = overflow;
EFLAGS.OF = overflow;
EFLAGS.SF = 0;
EFLAGS.ZF = 0;
EFLAGS.AF = 0;
EFLAGS.PF = 0;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1.17    | 0.86(1/1.17)     |
| 3C6000 | LA664 | 1       | 1                |

## x86mul.du

### Synopsis

```
Instruction: x86mul.du rj, rk
CPU Flags: LBT
```

### Description

x86-style multiply: multiply unsigned 64-bit values in `rj` and `rk`. Set CF and OF if overflow (result does not fit in 64-bits), clear other flags. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint64_t lhs = (uint64_t)a;
uint64_t rhs = (uint64_t)b;
unsigned __int128 product =
    (unsigned __int128)(uint64_t)lhs * (unsigned __int128)(uint64_t)rhs;
bool overflow = (product >> 64) != 0;
EFLAGS.CF = overflow;
EFLAGS.OF = overflow;
EFLAGS.SF = 0;
EFLAGS.ZF = 0;
EFLAGS.AF = 0;
EFLAGS.PF = 0;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1.17    | 0.86(1/1.17)     |
| 3C6000 | LA664 | 1       | 1                |
