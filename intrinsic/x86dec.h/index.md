# x86dec.h

LBT intrinsic · group: [x86 Integer](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/x86_integer/index.md)

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

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
