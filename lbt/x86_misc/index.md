# x86 Misc

## setx86j

### Synopsis

```
Instruction: setx86j rd, cond
CPU Flags: LBT
```

### Description

x86-style set if condition: evaluate x86 condition code `cond` against EFLAGS. Store 1 in `rd` if the condition holds, 0 otherwise.

### Operation

```
bool res = x86_cond_holds(EFLAGS, cond);
dst = res ? 1 : 0;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | N/A     | 1                |
| 3C6000 | LA664 | N/A     | 1                |

## setx86loope

### Synopsis

```
Instruction: setx86loope rd, rj
CPU Flags: LBT
```

### Description

x86-style loop while equal: the value in `rj` is the counter after decrement. Store 1 in `rd` if the counter is non-zero and ZF (in EFLAGS) is set, 0 otherwise.

### Operation

```
uint64_t counter_after_dec = a;
dst = (counter_after_dec != 0 && EFLAGS.ZF) ? 1 : 0;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 1                |
| 3C6000 | LA664 | 1       | 1                |

## setx86loopne

### Synopsis

```
Instruction: setx86loopne rd, rj
CPU Flags: LBT
```

### Description

x86-style loop while not equal: the value in `rj` is the counter after decrement. Store 1 in `rd` if the counter is non-zero and ZF (in EFLAGS) is clear, 0 otherwise.

### Operation

```
uint64_t counter_after_dec = a;
dst = (counter_after_dec != 0 && !EFLAGS.ZF) ? 1 : 0;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 1                |
| 3C6000 | LA664 | 1       | 1                |
