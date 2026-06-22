# LBT State Transfer

## movgr2scr

### Synopsis

```
Instruction: movgr2scr scrd, rj
CPU Flags: LBT
```

### Description

Move GPR to scratch register: copy value in `rj` to the scratch register selected by `scrd` (0-3). Scratch registers LBT0-LBT3 are accessible via $scr0-$scr3.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | N/A     | 0.5(1/2)         |
| 3C6000 | LA664 | N/A     | 1                |

## movscr2gr

### Synopsis

```
Instruction: movscr2gr rd, scrj
CPU Flags: LBT
```

### Description

Move scratch register to GPR: copy value in the scratch register selected by `scrj` (0-3) to `rd`.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | N/A     | 2                |
| 3C6000 | LA664 | N/A     | 1                |

## x86mfflag

### Synopsis

```
Instruction: x86mfflag rd, mask
CPU Flags: LBT
```

### Description

Move from x86 EFLAGS: read selected x86 flags (CF, PF, AF, ZF, SF, OF) from EFLAGS as determined by `mask`, pack them into `rd`.

### Operation

```
dst = 0;
if (mask & (1 << 0))
  dst |= (uint64_t)EFLAGS.CF << 0;
if (mask & (1 << 1))
  dst |= (uint64_t)EFLAGS.PF << 2;
if (mask & (1 << 2))
  dst |= (uint64_t)EFLAGS.AF << 4;
if (mask & (1 << 3))
  dst |= (uint64_t)EFLAGS.ZF << 6;
if (mask & (1 << 4))
  dst |= (uint64_t)EFLAGS.SF << 7;
if (mask & (1 << 5))
  dst |= (uint64_t)EFLAGS.OF << 11;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | N/A     | 1                |
| 3C6000 | LA664 | N/A     | 1                |

## x86mtflag

### Synopsis

```
Instruction: x86mtflag rd, mask
CPU Flags: LBT
```

### Description

Move to x86 EFLAGS: write selected x86 flags (CF, PF, AF, ZF, SF, OF) in EFLAGS from the corresponding bit positions in `rd`, as determined by `mask`.

### Operation

```
uint64_t v = a;
if (mask & (1 << 0))
  EFLAGS.CF = (v >> 0) & 1;
if (mask & (1 << 1))
  EFLAGS.PF = (v >> 2) & 1;
if (mask & (1 << 2))
  EFLAGS.AF = (v >> 4) & 1;
if (mask & (1 << 3))
  EFLAGS.ZF = (v >> 6) & 1;
if (mask & (1 << 4))
  EFLAGS.SF = (v >> 7) & 1;
if (mask & (1 << 5))
  EFLAGS.OF = (v >> 11) & 1;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | N/A     | 0.5(1/2)         |
| 3C6000 | LA664 | N/A     | 0.5(1/2)         |

## armmfflag

### Synopsis

```
Instruction: armmfflag rd, mask
CPU Flags: LBT
```

### Description

Move from ARMFLAGS: read selected ARM flags (N, Z, C, V) from ARMFLAGS as determined by `mask`, pack them into `rd`.

### Operation

```
dst = 0;
if (mask & (1 << 4))
  dst |= (uint64_t)ARMFLAGS.N << 31;
if (mask & (1 << 3))
  dst |= (uint64_t)ARMFLAGS.Z << 30;
if (mask & (1 << 0))
  dst |= (uint64_t)ARMFLAGS.C << 29;
if (mask & (1 << 5))
  dst |= (uint64_t)ARMFLAGS.V << 28;
dst = sext(dst, 32);
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | N/A     | 1                |
| 3C6000 | LA664 | N/A     | 1                |

## armmtflag

### Synopsis

```
Instruction: armmtflag rd, mask
CPU Flags: LBT
```

### Description

Move to ARMFLAGS: write selected ARM flags (N, Z, C, V) in ARMFLAGS from the corresponding bit positions in `rd`, as determined by `mask`.

### Operation

```
uint64_t v = a;
if (mask & (1 << 4))
  ARMFLAGS.N = (v >> 31) & 1;
if (mask & (1 << 3))
  ARMFLAGS.Z = (v >> 30) & 1;
if (mask & (1 << 0))
  ARMFLAGS.C = (v >> 29) & 1;
if (mask & (1 << 5))
  ARMFLAGS.V = (v >> 28) & 1;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | N/A     | 0.5(1/2)         |
| 3C6000 | LA664 | N/A     | 0.5(1/2)         |
