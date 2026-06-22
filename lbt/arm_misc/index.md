# ARM Misc

## armmove

### Synopsis

```
Instruction: armmove rd, rj, cond
CPU Flags: LBT
```

### Description

ARM-style conditional move: if the ARM condition holds, copy the value in `rj` to `rd`. ARMFLAGS are not updated.

### Operation

```
if (arm_cond_holds(ARMFLAGS, cond)) {
  dst = a;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 1                |
| 3C6000 | LA664 | 2       | 1                |

## armmov.w

### Synopsis

```
Instruction: armmov.w rj
CPU Flags: LBT
```

### Description

ARM-style move (with flag update): conditionally test the 32-bit value in `rj` and update ARMFLAGS (N, Z). Only ARMFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t r = a;
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

## armmov.d

### Synopsis

```
Instruction: armmov.d rj
CPU Flags: LBT
```

### Description

ARM-style move (with flag update): conditionally test the 64-bit value in `rj` and update ARMFLAGS (N, Z). Only ARMFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
if (arm_cond_holds(ARMFLAGS, cond)) {
  uint64_t r = a;
  ARMFLAGS.N = (r >> 63) & 1;
  ARMFLAGS.Z = r == 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## setarmj

### Synopsis

```
Instruction: setarmj rd, cond
CPU Flags: LBT
```

### Description

ARM-style set if condition: evaluate ARM condition code `cond` against ARMFLAGS. Store 1 in `rd` if the condition holds, 0 otherwise.

### Operation

```
bool res = arm_cond_holds(ARMFLAGS, cond);
dst = res ? 1 : 0;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | N/A     | 1                |
| 3C6000 | LA664 | N/A     | 1                |
