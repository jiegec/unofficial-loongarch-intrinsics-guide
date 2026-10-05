# x86 Shift

## x86rcl.b

### Synopsis

```
Instruction: x86rcl.b rj, rk
CPU Flags: LBT
```

### Description

x86-style rotate left through carry: rotate 8-bit value in `rj` and CF together as a 9-bit ring left by the amount specified in `rk`. Update CF, OF (for shift count of 1). Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint8_t v = (uint8_t)a;
unsigned c = (unsigned)(b & 0x1f);
unsigned n = c % 9;
if (n != 0) {
  unsigned carry_out = ((v >> (8 - n)) & 1);
  EFLAGS.CF = carry_out;
  if (c == 1) {
    uint8_t r = (uint8_t)(v << 1);
    EFLAGS.OF = ((r & 0x80) != 0) != carry_out;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86rcl.h

### Synopsis

```
Instruction: x86rcl.h rj, rk
CPU Flags: LBT
```

### Description

x86-style rotate left through carry: rotate 16-bit value in `rj` and CF together as a 17-bit ring left by the amount specified in `rk`. Update CF, OF (for shift count of 1). Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint16_t v = (uint16_t)a;
unsigned c = (unsigned)(b & 0x1f);
unsigned n = c % 17;
if (n != 0) {
  unsigned carry_out = ((v >> (16 - n)) & 1);
  EFLAGS.CF = carry_out;
  if (c == 1) {
    uint16_t r = (uint16_t)(v << 1);
    EFLAGS.OF = ((r & 0x8000) != 0) != carry_out;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86rcl.w

### Synopsis

```
Instruction: x86rcl.w rj, rk
CPU Flags: LBT
```

### Description

x86-style rotate left through carry: rotate 32-bit value in `rj` and CF together as a 33-bit ring left by the amount specified in `rk`. Update CF, OF (for shift count of 1). Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint32_t v = (uint32_t)a;
unsigned c = (unsigned)(b & 0x1f);
unsigned n = c;
if (n != 0) {
  unsigned carry_out = ((v >> (32 - n)) & 1);
  EFLAGS.CF = carry_out;
  if (c == 1) {
    uint32_t r = (uint32_t)(v << 1);
    EFLAGS.OF = ((r & 0x80000000) != 0) != carry_out;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86rcl.d

### Synopsis

```
Instruction: x86rcl.d rj, rk
CPU Flags: LBT
```

### Description

x86-style rotate left through carry: rotate 64-bit value in `rj` and CF together as a 65-bit ring left by the amount specified in `rk`. Update CF, OF (for shift count of 1). Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint64_t v = (uint64_t)a;
unsigned c = (unsigned)(b & 0x3f);
unsigned n = c;
if (n != 0) {
  unsigned carry_out = ((v >> (64 - n)) & 1);
  EFLAGS.CF = carry_out;
  if (c == 1) {
    uint64_t r = (uint64_t)(v << 1);
    EFLAGS.OF = ((r & 0x8000000000000000ULL) != 0) != carry_out;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86rcli.b

### Synopsis

```
Instruction: x86rcli.b rj, imm
CPU Flags: LBT
```

### Description

x86-style rotate left through carry: rotate 8-bit value in `rj` and CF together as a 9-bit ring left by immediate `imm`. Update CF, OF (for shift count of 1). Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint8_t v = (uint8_t)a;
unsigned c = (unsigned)(imm & 0x1f);
unsigned n = c % 9;
if (n != 0) {
  unsigned carry_out = ((v >> (8 - n)) & 1);
  EFLAGS.CF = carry_out;
  if (c == 1) {
    uint8_t r = (uint8_t)(v << 1);
    EFLAGS.OF = ((r & 0x80) != 0) != carry_out;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86rcli.h

### Synopsis

```
Instruction: x86rcli.h rj, imm
CPU Flags: LBT
```

### Description

x86-style rotate left through carry: rotate 16-bit value in `rj` and CF together as a 17-bit ring left by immediate `imm`. Update CF, OF (for shift count of 1). Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint16_t v = (uint16_t)a;
unsigned c = (unsigned)(imm & 0x1f);
unsigned n = c % 17;
if (n != 0) {
  unsigned carry_out = ((v >> (16 - n)) & 1);
  EFLAGS.CF = carry_out;
  if (c == 1) {
    uint16_t r = (uint16_t)(v << 1);
    EFLAGS.OF = ((r & 0x8000) != 0) != carry_out;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86rcli.w

### Synopsis

```
Instruction: x86rcli.w rj, imm
CPU Flags: LBT
```

### Description

x86-style rotate left through carry: rotate 32-bit value in `rj` and CF together as a 33-bit ring left by immediate `imm`. Update CF, OF (for shift count of 1). Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint32_t v = (uint32_t)a;
unsigned c = (unsigned)(imm & 0x1f);
unsigned n = c;
if (n != 0) {
  unsigned carry_out = ((v >> (32 - n)) & 1);
  EFLAGS.CF = carry_out;
  if (c == 1) {
    uint32_t r = (uint32_t)(v << 1);
    EFLAGS.OF = ((r & 0x80000000) != 0) != carry_out;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86rcli.d

### Synopsis

```
Instruction: x86rcli.d rj, imm
CPU Flags: LBT
```

### Description

x86-style rotate left through carry: rotate 64-bit value in `rj` and CF together as a 65-bit ring left by immediate `imm`. Update CF, OF (for shift count of 1). Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint64_t v = (uint64_t)a;
unsigned c = (unsigned)(imm & 0x3f);
unsigned n = c;
if (n != 0) {
  unsigned carry_out = ((v >> (64 - n)) & 1);
  EFLAGS.CF = carry_out;
  if (c == 1) {
    uint64_t r = (uint64_t)(v << 1);
    EFLAGS.OF = ((r & 0x8000000000000000ULL) != 0) != carry_out;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86rcr.b

### Synopsis

```
Instruction: x86rcr.b rj, rk
CPU Flags: LBT
```

### Description

x86-style rotate right through carry: rotate 8-bit value in `rj` and CF together as a 9-bit ring right by the amount specified in `rk`. Update CF, OF (for shift count of 1). Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint8_t v = (uint8_t)a;
unsigned c = (unsigned)(b & 0x1f);
unsigned n = c % 9;
if (n != 0) {
  unsigned carry_in = EFLAGS.CF;
  unsigned carry_out = ((v >> ((n - 1) % (8 + 1))) & 1);
  EFLAGS.CF = carry_out;
  if (c == 1) {
    uint8_t r = (uint8_t)((v >> 1) | (carry_in << 7));
    EFLAGS.OF = (((r >> 7) ^ (r >> 6)) & 1) != 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86rcr.h

### Synopsis

```
Instruction: x86rcr.h rj, rk
CPU Flags: LBT
```

### Description

x86-style rotate right through carry: rotate 16-bit value in `rj` and CF together as a 17-bit ring right by the amount specified in `rk`. Update CF, OF (for shift count of 1). Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint16_t v = (uint16_t)a;
unsigned c = (unsigned)(b & 0x1f);
unsigned n = c % 17;
if (n != 0) {
  unsigned carry_in = EFLAGS.CF;
  unsigned carry_out = ((v >> ((n - 1) % (16 + 1))) & 1);
  EFLAGS.CF = carry_out;
  if (c == 1) {
    uint16_t r = (uint16_t)((v >> 1) | (carry_in << 15));
    EFLAGS.OF = (((r >> 15) ^ (r >> 14)) & 1) != 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86rcr.w

### Synopsis

```
Instruction: x86rcr.w rj, rk
CPU Flags: LBT
```

### Description

x86-style rotate right through carry: rotate 32-bit value in `rj` and CF together as a 33-bit ring right by the amount specified in `rk`. Update CF, OF (for shift count of 1). Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint32_t v = (uint32_t)a;
unsigned c = (unsigned)(b & 0x1f);
unsigned n = c;
if (n != 0) {
  unsigned carry_in = EFLAGS.CF;
  unsigned carry_out = ((v >> ((n - 1) % (32 + 1))) & 1);
  EFLAGS.CF = carry_out;
  if (c == 1) {
    uint32_t r = (uint32_t)((v >> 1) | (carry_in << 31));
    EFLAGS.OF = (((r >> 31) ^ (r >> 30)) & 1) != 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86rcr.d

### Synopsis

```
Instruction: x86rcr.d rj, rk
CPU Flags: LBT
```

### Description

x86-style rotate right through carry: rotate 64-bit value in `rj` and CF together as a 65-bit ring right by the amount specified in `rk`. Update CF, OF (for shift count of 1). Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint64_t v = (uint64_t)a;
unsigned c = (unsigned)(b & 0x3f);
unsigned n = c;
if (n != 0) {
  unsigned carry_in = EFLAGS.CF;
  unsigned carry_out = ((v >> ((n - 1) % (64 + 1))) & 1);
  EFLAGS.CF = carry_out;
  if (c == 1) {
    uint64_t r = (uint64_t)((v >> 1) | ((uint64_t)carry_in << 63));
    EFLAGS.OF = (((r >> 63) ^ (r >> 62)) & 1) != 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86rcri.b

### Synopsis

```
Instruction: x86rcri.b rj, imm
CPU Flags: LBT
```

### Description

x86-style rotate right through carry: rotate 8-bit value in `rj` and CF together as a 9-bit ring right by immediate `imm`. Update CF, OF (for shift count of 1). Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint8_t v = (uint8_t)a;
unsigned c = (unsigned)(imm & 0x1f);
unsigned n = c % 9;
if (n != 0) {
  unsigned carry_in = EFLAGS.CF;
  unsigned carry_out = ((v >> ((n - 1) % (8 + 1))) & 1);
  EFLAGS.CF = carry_out;
  if (c == 1) {
    uint8_t r = (uint8_t)((v >> 1) | (carry_in << 7));
    EFLAGS.OF = (((r >> 7) ^ (r >> 6)) & 1) != 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86rcri.h

### Synopsis

```
Instruction: x86rcri.h rj, imm
CPU Flags: LBT
```

### Description

x86-style rotate right through carry: rotate 16-bit value in `rj` and CF together as a 17-bit ring right by immediate `imm`. Update CF, OF (for shift count of 1). Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint16_t v = (uint16_t)a;
unsigned c = (unsigned)(imm & 0x1f);
unsigned n = c % 17;
if (n != 0) {
  unsigned carry_in = EFLAGS.CF;
  unsigned carry_out = ((v >> ((n - 1) % (16 + 1))) & 1);
  EFLAGS.CF = carry_out;
  if (c == 1) {
    uint16_t r = (uint16_t)((v >> 1) | (carry_in << 15));
    EFLAGS.OF = (((r >> 15) ^ (r >> 14)) & 1) != 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86rcri.w

### Synopsis

```
Instruction: x86rcri.w rj, imm
CPU Flags: LBT
```

### Description

x86-style rotate right through carry: rotate 32-bit value in `rj` and CF together as a 33-bit ring right by immediate `imm`. Update CF, OF (for shift count of 1). Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint32_t v = (uint32_t)a;
unsigned c = (unsigned)(imm & 0x1f);
unsigned n = c;
if (n != 0) {
  unsigned carry_in = EFLAGS.CF;
  unsigned carry_out = ((v >> ((n - 1) % (32 + 1))) & 1);
  EFLAGS.CF = carry_out;
  if (c == 1) {
    uint32_t r = (uint32_t)((v >> 1) | (carry_in << 31));
    EFLAGS.OF = (((r >> 31) ^ (r >> 30)) & 1) != 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86rcri.d

### Synopsis

```
Instruction: x86rcri.d rj, imm
CPU Flags: LBT
```

### Description

x86-style rotate right through carry: rotate 64-bit value in `rj` and CF together as a 65-bit ring right by immediate `imm`. Update CF, OF (for shift count of 1). Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint64_t v = (uint64_t)a;
unsigned c = (unsigned)(imm & 0x3f);
unsigned n = c;
if (n != 0) {
  unsigned carry_in = EFLAGS.CF;
  unsigned carry_out = ((v >> ((n - 1) % (64 + 1))) & 1);
  EFLAGS.CF = carry_out;
  if (c == 1) {
    uint64_t r = (uint64_t)((v >> 1) | ((uint64_t)carry_in << 63));
    EFLAGS.OF = (((r >> 63) ^ (r >> 62)) & 1) != 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86rotl.b

### Synopsis

```
Instruction: x86rotl.b rj, rk
CPU Flags: LBT
```

### Description

x86-style rotate left: rotate 8-bit value in `rj` left by the amount specified in `rk`. Update CF, OF (for shift count of 1). Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint8_t v = (uint8_t)a;
unsigned c = (unsigned)(b & 0x1f);
unsigned n = c % 8;
uint8_t r = (uint8_t)((v << n) | (v >> (8 - n)));
EFLAGS.CF = ((v >> ((8 - n) % 8)) & 1);
if (c == 1) {
  EFLAGS.OF = ((v ^ r) & 0x80) != 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86rotl.h

### Synopsis

```
Instruction: x86rotl.h rj, rk
CPU Flags: LBT
```

### Description

x86-style rotate left: rotate 16-bit value in `rj` left by the amount specified in `rk`. Update CF, OF (for shift count of 1). Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint16_t v = (uint16_t)a;
unsigned c = (unsigned)(b & 0x1f);
unsigned n = c % 16;
uint16_t r = (uint16_t)((v << n) | (v >> (16 - n)));
EFLAGS.CF = ((v >> ((16 - n) % 16)) & 1);
if (c == 1) {
  EFLAGS.OF = ((v ^ r) & 0x8000) != 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86rotl.w

### Synopsis

```
Instruction: x86rotl.w rj, rk
CPU Flags: LBT
```

### Description

x86-style rotate left: rotate 32-bit value in `rj` left by the amount specified in `rk`. Update CF, OF (for shift count of 1). Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint32_t v = (uint32_t)a;
unsigned c = (unsigned)(b & 0x1f);
unsigned n = c % 32;
uint32_t r = (uint32_t)((v << n) | (v >> (32 - n)));
EFLAGS.CF = ((v >> ((32 - n) % 32)) & 1);
if (c == 1) {
  EFLAGS.OF = ((v ^ r) & 0x80000000) != 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86rotl.d

### Synopsis

```
Instruction: x86rotl.d rj, rk
CPU Flags: LBT
```

### Description

x86-style rotate left: rotate 64-bit value in `rj` left by the amount specified in `rk`. Update CF, OF (for shift count of 1). Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint64_t v = (uint64_t)a;
unsigned c = (unsigned)(b & 0x3f);
unsigned n = c % 64;
uint64_t r = (uint64_t)((v << n) | (v >> (64 - n)));
EFLAGS.CF = ((v >> ((64 - n) % 64)) & 1);
if (c == 1) {
  EFLAGS.OF = ((v ^ r) & 0x8000000000000000ULL) != 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86rotli.b

### Synopsis

```
Instruction: x86rotli.b rj, imm
CPU Flags: LBT
```

### Description

x86-style rotate left: rotate 8-bit value in `rj` left by immediate `imm`. Update CF, OF (for shift count of 1). Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint8_t v = (uint8_t)a;
unsigned c = (unsigned)imm;
unsigned n = c % 8;
uint8_t r = (uint8_t)((v << n) | (v >> (8 - n)));
EFLAGS.CF = ((v >> ((8 - n) % 8)) & 1);
if (c == 1) {
  EFLAGS.OF = ((v ^ r) & 0x80) != 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86rotli.h

### Synopsis

```
Instruction: x86rotli.h rj, imm
CPU Flags: LBT
```

### Description

x86-style rotate left: rotate 16-bit value in `rj` left by immediate `imm`. Update CF, OF (for shift count of 1). Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint16_t v = (uint16_t)a;
unsigned c = (unsigned)imm;
unsigned n = c % 16;
uint16_t r = (uint16_t)((v << n) | (v >> (16 - n)));
EFLAGS.CF = ((v >> ((16 - n) % 16)) & 1);
if (c == 1) {
  EFLAGS.OF = ((v ^ r) & 0x8000) != 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86rotli.w

### Synopsis

```
Instruction: x86rotli.w rj, imm
CPU Flags: LBT
```

### Description

x86-style rotate left: rotate 32-bit value in `rj` left by immediate `imm`. Update CF, OF (for shift count of 1). Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint32_t v = (uint32_t)a;
unsigned c = (unsigned)imm;
unsigned n = c % 32;
uint32_t r = (uint32_t)((v << n) | (v >> (32 - n)));
EFLAGS.CF = ((v >> ((32 - n) % 32)) & 1);
if (c == 1) {
  EFLAGS.OF = ((v ^ r) & 0x80000000) != 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86rotli.d

### Synopsis

```
Instruction: x86rotli.d rj, imm
CPU Flags: LBT
```

### Description

x86-style rotate left: rotate 64-bit value in `rj` left by immediate `imm`. Update CF, OF (for shift count of 1). Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint64_t v = (uint64_t)a;
unsigned c = (unsigned)imm;
unsigned n = c % 64;
uint64_t r = (uint64_t)((v << n) | (v >> (64 - n)));
EFLAGS.CF = ((v >> ((64 - n) % 64)) & 1);
if (c == 1) {
  EFLAGS.OF = ((v ^ r) & 0x8000000000000000ULL) != 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86rotr.b

### Synopsis

```
Instruction: x86rotr.b rj, rk
CPU Flags: LBT
```

### Description

x86-style rotate right: rotate 8-bit value in `rj` right by the amount specified in `rk`. Update CF, OF (for shift count of 1). Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint8_t v = (uint8_t)a;
unsigned c = (unsigned)(b & 0x1f);
unsigned n = c % 8;
uint8_t r = (uint8_t)((v >> n) | (v << (8 - n)));
EFLAGS.CF = ((v >> ((n - 1 + 8) % 8)) & 1);
if (c == 1) {
  EFLAGS.OF = (((v ^ r) & 0x80) != 0);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86rotr.h

### Synopsis

```
Instruction: x86rotr.h rj, rk
CPU Flags: LBT
```

### Description

x86-style rotate right: rotate 16-bit value in `rj` right by the amount specified in `rk`. Update CF, OF (for shift count of 1). Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint16_t v = (uint16_t)a;
unsigned c = (unsigned)(b & 0x1f);
unsigned n = c % 16;
uint16_t r = (uint16_t)((v >> n) | (v << (16 - n)));
EFLAGS.CF = ((v >> ((n - 1 + 16) % 16)) & 1);
if (c == 1) {
  EFLAGS.OF = (((v ^ r) & 0x8000) != 0);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86rotr.w

### Synopsis

```
Instruction: x86rotr.w rj, rk
CPU Flags: LBT
```

### Description

x86-style rotate right: rotate 32-bit value in `rj` right by the amount specified in `rk`. Update CF, OF (for shift count of 1). Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint32_t v = (uint32_t)a;
unsigned c = (unsigned)(b & 0x1f);
unsigned n = c % 32;
uint32_t r = (uint32_t)((v >> n) | (v << (32 - n)));
EFLAGS.CF = ((v >> ((n - 1 + 32) % 32)) & 1);
if (c == 1) {
  EFLAGS.OF = (((v ^ r) & 0x80000000) != 0);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86rotr.d

### Synopsis

```
Instruction: x86rotr.d rj, rk
CPU Flags: LBT
```

### Description

x86-style rotate right: rotate 64-bit value in `rj` right by the amount specified in `rk`. Update CF, OF (for shift count of 1). Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint64_t v = (uint64_t)a;
unsigned c = (unsigned)(b & 0x3f);
unsigned n = c % 64;
uint64_t r = (uint64_t)((v >> n) | (v << (64 - n)));
EFLAGS.CF = ((v >> ((n - 1 + 64) % 64)) & 1);
if (c == 1) {
  EFLAGS.OF = (((v ^ r) & 0x8000000000000000ULL) != 0);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86rotri.b

### Synopsis

```
Instruction: x86rotri.b rj, imm
CPU Flags: LBT
```

### Description

x86-style rotate right: rotate 8-bit value in `rj` right by immediate `imm`. Update CF, OF (for shift count of 1). Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint8_t v = (uint8_t)a;
unsigned c = (unsigned)imm;
unsigned n = c % 8;
uint8_t r = (uint8_t)((v >> n) | (v << (8 - n)));
EFLAGS.CF = ((v >> ((n - 1 + 8) % 8)) & 1);
if (c == 1) {
  EFLAGS.OF = (((v ^ r) & 0x80) != 0);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86rotri.h

### Synopsis

```
Instruction: x86rotri.h rj, imm
CPU Flags: LBT
```

### Description

x86-style rotate right: rotate 16-bit value in `rj` right by immediate `imm`. Update CF, OF (for shift count of 1). Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint16_t v = (uint16_t)a;
unsigned c = (unsigned)imm;
unsigned n = c % 16;
uint16_t r = (uint16_t)((v >> n) | (v << (16 - n)));
EFLAGS.CF = ((v >> ((n - 1 + 16) % 16)) & 1);
if (c == 1) {
  EFLAGS.OF = (((v ^ r) & 0x8000) != 0);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86rotri.w

### Synopsis

```
Instruction: x86rotri.w rj, imm
CPU Flags: LBT
```

### Description

x86-style rotate right: rotate 32-bit value in `rj` right by immediate `imm`. Update CF, OF (for shift count of 1). Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint32_t v = (uint32_t)a;
unsigned c = (unsigned)imm;
unsigned n = c % 32;
uint32_t r = (uint32_t)((v >> n) | (v << (32 - n)));
EFLAGS.CF = ((v >> ((n - 1 + 32) % 32)) & 1);
if (c == 1) {
  EFLAGS.OF = (((v ^ r) & 0x80000000) != 0);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86rotri.d

### Synopsis

```
Instruction: x86rotri.d rj, imm
CPU Flags: LBT
```

### Description

x86-style rotate right: rotate 64-bit value in `rj` right by immediate `imm`. Update CF, OF (for shift count of 1). Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint64_t v = (uint64_t)a;
unsigned c = (unsigned)imm;
unsigned n = c % 64;
uint64_t r = (uint64_t)((v >> n) | (v << (64 - n)));
EFLAGS.CF = ((v >> ((n - 1 + 64) % 64)) & 1);
if (c == 1) {
  EFLAGS.OF = (((v ^ r) & 0x8000000000000000ULL) != 0);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86sll.b

### Synopsis

```
Instruction: x86sll.b rj, rk
CPU Flags: LBT
```

### Description

x86-style shift left logical: shift 8-bit value in `rj` left by the amount specified in `rk`. If shift count is non-zero, update CF, PF, ZF, SF (and OF if count is 1). If shift count is zero, flags are unchanged. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint8_t v = (uint8_t)a;
unsigned c = (unsigned)(b & 0x1f);
if (c != 0) {
  uint8_t carry_out = c > 8 ? 0 : ((v >> (8 - c)) & 1);
  uint8_t r = c >= 8 ? 0 : (uint8_t)(v << c);
  EFLAGS.CF = carry_out;
  EFLAGS.PF = parity_even((uint8_t)r);
  EFLAGS.ZF = r == 0;
  EFLAGS.SF = (int8_t)r < 0;
  if (c == 1) {
    EFLAGS.OF = ((v ^ r) & 0x80) != 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86sll.h

### Synopsis

```
Instruction: x86sll.h rj, rk
CPU Flags: LBT
```

### Description

x86-style shift left logical: shift 16-bit value in `rj` left by the amount specified in `rk`. If shift count is non-zero, update CF, PF, ZF, SF (and OF if count is 1). If shift count is zero, flags are unchanged. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint16_t v = (uint16_t)a;
unsigned c = (unsigned)(b & 0x1f);
if (c != 0) {
  uint8_t carry_out = c > 16 ? 0 : ((v >> (16 - c)) & 1);
  uint16_t r = c >= 16 ? 0 : (uint16_t)(v << c);
  EFLAGS.CF = carry_out;
  EFLAGS.PF = parity_even((uint8_t)r);
  EFLAGS.ZF = r == 0;
  EFLAGS.SF = (int16_t)r < 0;
  if (c == 1) {
    EFLAGS.OF = ((v ^ r) & 0x8000) != 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86sll.w

### Synopsis

```
Instruction: x86sll.w rj, rk
CPU Flags: LBT
```

### Description

x86-style shift left logical: shift 32-bit value in `rj` left by the amount specified in `rk`. If shift count is non-zero, update CF, PF, ZF, SF (and OF if count is 1). If shift count is zero, flags are unchanged. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint32_t v = (uint32_t)a;
unsigned c = (unsigned)(b & 0x1f);
if (c != 0) {
  uint8_t carry_out = c > 32 ? 0 : ((v >> (32 - c)) & 1);
  uint32_t r = c >= 32 ? 0 : (uint32_t)(v << c);
  EFLAGS.CF = carry_out;
  EFLAGS.PF = parity_even((uint8_t)r);
  EFLAGS.ZF = r == 0;
  EFLAGS.SF = (int32_t)r < 0;
  if (c == 1) {
    EFLAGS.OF = ((v ^ r) & 0x80000000) != 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86sll.d

### Synopsis

```
Instruction: x86sll.d rj, rk
CPU Flags: LBT
```

### Description

x86-style shift left logical: shift 64-bit value in `rj` left by the amount specified in `rk`. If shift count is non-zero, update CF, PF, ZF, SF (and OF if count is 1). If shift count is zero, flags are unchanged. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint64_t v = (uint64_t)a;
unsigned c = (unsigned)(b & 0x3f);
if (c != 0) {
  uint8_t carry_out = c > 64 ? 0 : ((v >> (64 - c)) & 1);
  uint64_t r = c >= 64 ? 0 : (uint64_t)(v << c);
  EFLAGS.CF = carry_out;
  EFLAGS.PF = parity_even((uint8_t)r);
  EFLAGS.ZF = r == 0;
  EFLAGS.SF = (int64_t)r < 0;
  if (c == 1) {
    EFLAGS.OF = ((v ^ r) & 0x8000000000000000ULL) != 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86slli.b

### Synopsis

```
Instruction: x86slli.b rj, imm
CPU Flags: LBT
```

### Description

x86-style shift left logical: shift 8-bit value in `rj` left by immediate `imm`. If shift count is non-zero, update CF, PF, ZF, SF (and OF if count is 1). If shift count is zero, flags are unchanged. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint8_t v = (uint8_t)a;
unsigned c = (unsigned)imm;
if (c != 0) {
  uint8_t carry_out = c > 8 ? 0 : ((v >> (8 - c)) & 1);
  uint8_t r = c >= 8 ? 0 : (uint8_t)(v << c);
  EFLAGS.CF = carry_out;
  EFLAGS.PF = parity_even((uint8_t)r);
  EFLAGS.ZF = r == 0;
  EFLAGS.SF = (int8_t)r < 0;
  if (c == 1) {
    EFLAGS.OF = ((v ^ r) & 0x80) != 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86slli.h

### Synopsis

```
Instruction: x86slli.h rj, imm
CPU Flags: LBT
```

### Description

x86-style shift left logical: shift 16-bit value in `rj` left by immediate `imm`. If shift count is non-zero, update CF, PF, ZF, SF (and OF if count is 1). If shift count is zero, flags are unchanged. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint16_t v = (uint16_t)a;
unsigned c = (unsigned)imm;
if (c != 0) {
  uint8_t carry_out = c > 16 ? 0 : ((v >> (16 - c)) & 1);
  uint16_t r = c >= 16 ? 0 : (uint16_t)(v << c);
  EFLAGS.CF = carry_out;
  EFLAGS.PF = parity_even((uint8_t)r);
  EFLAGS.ZF = r == 0;
  EFLAGS.SF = (int16_t)r < 0;
  if (c == 1) {
    EFLAGS.OF = ((v ^ r) & 0x8000) != 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86slli.w

### Synopsis

```
Instruction: x86slli.w rj, imm
CPU Flags: LBT
```

### Description

x86-style shift left logical: shift 32-bit value in `rj` left by immediate `imm`. If shift count is non-zero, update CF, PF, ZF, SF (and OF if count is 1). If shift count is zero, flags are unchanged. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint32_t v = (uint32_t)a;
unsigned c = (unsigned)imm;
if (c != 0) {
  uint8_t carry_out = c > 32 ? 0 : ((v >> (32 - c)) & 1);
  uint32_t r = c >= 32 ? 0 : (uint32_t)(v << c);
  EFLAGS.CF = carry_out;
  EFLAGS.PF = parity_even((uint8_t)r);
  EFLAGS.ZF = r == 0;
  EFLAGS.SF = (int32_t)r < 0;
  if (c == 1) {
    EFLAGS.OF = ((v ^ r) & 0x80000000) != 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86slli.d

### Synopsis

```
Instruction: x86slli.d rj, imm
CPU Flags: LBT
```

### Description

x86-style shift left logical: shift 64-bit value in `rj` left by immediate `imm`. If shift count is non-zero, update CF, PF, ZF, SF (and OF if count is 1). If shift count is zero, flags are unchanged. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint64_t v = (uint64_t)a;
unsigned c = (unsigned)imm;
if (c != 0) {
  uint8_t carry_out = c > 64 ? 0 : ((v >> (64 - c)) & 1);
  uint64_t r = c >= 64 ? 0 : (uint64_t)(v << c);
  EFLAGS.CF = carry_out;
  EFLAGS.PF = parity_even((uint8_t)r);
  EFLAGS.ZF = r == 0;
  EFLAGS.SF = (int64_t)r < 0;
  if (c == 1) {
    EFLAGS.OF = ((v ^ r) & 0x8000000000000000ULL) != 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86srl.b

### Synopsis

```
Instruction: x86srl.b rj, rk
CPU Flags: LBT
```

### Description

x86-style shift right logical: shift 8-bit value in `rj` right logically by the amount specified in `rk`. If shift count is non-zero, update CF, PF, ZF, SF (and OF if count is 1). If shift count is zero, flags are unchanged. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint8_t v = (uint8_t)a;
unsigned c = (unsigned)(b & 0x1f);
if (c != 0) {
  uint8_t carry_out = c > 8 ? 0 : ((v >> (c - 1)) & 1);
  uint8_t r = c >= 8 ? 0 : (v >> c);
  EFLAGS.CF = carry_out;
  EFLAGS.PF = parity_even((uint8_t)r);
  EFLAGS.ZF = r == 0;
  EFLAGS.SF = (int8_t)r < 0;
  if (c == 1) {
    EFLAGS.OF = ((v ^ r) & 0x80) != 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86srl.h

### Synopsis

```
Instruction: x86srl.h rj, rk
CPU Flags: LBT
```

### Description

x86-style shift right logical: shift 16-bit value in `rj` right logically by the amount specified in `rk`. If shift count is non-zero, update CF, PF, ZF, SF (and OF if count is 1). If shift count is zero, flags are unchanged. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint16_t v = (uint16_t)a;
unsigned c = (unsigned)(b & 0x1f);
if (c != 0) {
  uint8_t carry_out = c > 16 ? 0 : ((v >> (c - 1)) & 1);
  uint16_t r = c >= 16 ? 0 : (v >> c);
  EFLAGS.CF = carry_out;
  EFLAGS.PF = parity_even((uint8_t)r);
  EFLAGS.ZF = r == 0;
  EFLAGS.SF = (int16_t)r < 0;
  if (c == 1) {
    EFLAGS.OF = ((v ^ r) & 0x8000) != 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86srl.w

### Synopsis

```
Instruction: x86srl.w rj, rk
CPU Flags: LBT
```

### Description

x86-style shift right logical: shift 32-bit value in `rj` right logically by the amount specified in `rk`. If shift count is non-zero, update CF, PF, ZF, SF (and OF if count is 1). If shift count is zero, flags are unchanged. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint32_t v = (uint32_t)a;
unsigned c = (unsigned)(b & 0x1f);
if (c != 0) {
  uint8_t carry_out = c > 32 ? 0 : ((v >> (c - 1)) & 1);
  uint32_t r = c >= 32 ? 0 : (v >> c);
  EFLAGS.CF = carry_out;
  EFLAGS.PF = parity_even((uint8_t)r);
  EFLAGS.ZF = r == 0;
  EFLAGS.SF = (int32_t)r < 0;
  if (c == 1) {
    EFLAGS.OF = ((v ^ r) & 0x80000000) != 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86srl.d

### Synopsis

```
Instruction: x86srl.d rj, rk
CPU Flags: LBT
```

### Description

x86-style shift right logical: shift 64-bit value in `rj` right logically by the amount specified in `rk`. If shift count is non-zero, update CF, PF, ZF, SF (and OF if count is 1). If shift count is zero, flags are unchanged. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint64_t v = (uint64_t)a;
unsigned c = (unsigned)(b & 0x3f);
if (c != 0) {
  uint8_t carry_out = c > 64 ? 0 : ((v >> (c - 1)) & 1);
  uint64_t r = c >= 64 ? 0 : (v >> c);
  EFLAGS.CF = carry_out;
  EFLAGS.PF = parity_even((uint8_t)r);
  EFLAGS.ZF = r == 0;
  EFLAGS.SF = (int64_t)r < 0;
  if (c == 1) {
    EFLAGS.OF = ((v ^ r) & 0x8000000000000000ULL) != 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86srli.b

### Synopsis

```
Instruction: x86srli.b rj, imm
CPU Flags: LBT
```

### Description

x86-style shift right logical: shift 8-bit value in `rj` right logically by immediate `imm`. If shift count is non-zero, update CF, PF, ZF, SF (and OF if count is 1). If shift count is zero, flags are unchanged. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint8_t v = (uint8_t)a;
unsigned c = (unsigned)imm;
if (c != 0) {
  uint8_t carry_out = c > 8 ? 0 : ((v >> (c - 1)) & 1);
  uint8_t r = c >= 8 ? 0 : (v >> c);
  EFLAGS.CF = carry_out;
  EFLAGS.PF = parity_even((uint8_t)r);
  EFLAGS.ZF = r == 0;
  EFLAGS.SF = (int8_t)r < 0;
  if (c == 1) {
    EFLAGS.OF = ((v ^ r) & 0x80) != 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86srli.h

### Synopsis

```
Instruction: x86srli.h rj, imm
CPU Flags: LBT
```

### Description

x86-style shift right logical: shift 16-bit value in `rj` right logically by immediate `imm`. If shift count is non-zero, update CF, PF, ZF, SF (and OF if count is 1). If shift count is zero, flags are unchanged. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint16_t v = (uint16_t)a;
unsigned c = (unsigned)imm;
if (c != 0) {
  uint8_t carry_out = c > 16 ? 0 : ((v >> (c - 1)) & 1);
  uint16_t r = c >= 16 ? 0 : (v >> c);
  EFLAGS.CF = carry_out;
  EFLAGS.PF = parity_even((uint8_t)r);
  EFLAGS.ZF = r == 0;
  EFLAGS.SF = (int16_t)r < 0;
  if (c == 1) {
    EFLAGS.OF = ((v ^ r) & 0x8000) != 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86srli.w

### Synopsis

```
Instruction: x86srli.w rj, imm
CPU Flags: LBT
```

### Description

x86-style shift right logical: shift 32-bit value in `rj` right logically by immediate `imm`. If shift count is non-zero, update CF, PF, ZF, SF (and OF if count is 1). If shift count is zero, flags are unchanged. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint32_t v = (uint32_t)a;
unsigned c = (unsigned)imm;
if (c != 0) {
  uint8_t carry_out = c > 32 ? 0 : ((v >> (c - 1)) & 1);
  uint32_t r = c >= 32 ? 0 : (v >> c);
  EFLAGS.CF = carry_out;
  EFLAGS.PF = parity_even((uint8_t)r);
  EFLAGS.ZF = r == 0;
  EFLAGS.SF = (int32_t)r < 0;
  if (c == 1) {
    EFLAGS.OF = ((v ^ r) & 0x80000000) != 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86srli.d

### Synopsis

```
Instruction: x86srli.d rj, imm
CPU Flags: LBT
```

### Description

x86-style shift right logical: shift 64-bit value in `rj` right logically by immediate `imm`. If shift count is non-zero, update CF, PF, ZF, SF (and OF if count is 1). If shift count is zero, flags are unchanged. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint64_t v = (uint64_t)a;
unsigned c = (unsigned)imm;
if (c != 0) {
  uint8_t carry_out = c > 64 ? 0 : ((v >> (c - 1)) & 1);
  uint64_t r = c >= 64 ? 0 : (v >> c);
  EFLAGS.CF = carry_out;
  EFLAGS.PF = parity_even((uint8_t)r);
  EFLAGS.ZF = r == 0;
  EFLAGS.SF = (int64_t)r < 0;
  if (c == 1) {
    EFLAGS.OF = ((v ^ r) & 0x8000000000000000ULL) != 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86sra.b

### Synopsis

```
Instruction: x86sra.b rj, rk
CPU Flags: LBT
```

### Description

x86-style shift right arithmetic: shift 8-bit value in `rj` right arithmetically by the amount specified in `rk`. If shift count is non-zero, update CF, PF, ZF, SF (OF is set to 0 if count is 1). If shift count is zero, flags are unchanged. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint8_t v = (uint8_t)a;
unsigned c = (unsigned)(b & 0x1f);
if (c != 0) {
  uint8_t carry_out = c > 8 ? ((int8_t)v < 0) : ((v >> (c - 1)) & 1);
  uint8_t r =
      c >= 8 ? (uint8_t)((int8_t)v >> (8 - 1)) : (uint8_t)((int8_t)v >> c);
  EFLAGS.CF = carry_out;
  EFLAGS.PF = parity_even((uint8_t)r);
  EFLAGS.ZF = r == 0;
  EFLAGS.SF = (int8_t)r < 0;
  if (c == 1) {
    EFLAGS.OF = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86sra.h

### Synopsis

```
Instruction: x86sra.h rj, rk
CPU Flags: LBT
```

### Description

x86-style shift right arithmetic: shift 16-bit value in `rj` right arithmetically by the amount specified in `rk`. If shift count is non-zero, update CF, PF, ZF, SF (OF is set to 0 if count is 1). If shift count is zero, flags are unchanged. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint16_t v = (uint16_t)a;
unsigned c = (unsigned)(b & 0x1f);
if (c != 0) {
  uint8_t carry_out = c > 16 ? ((int16_t)v < 0) : ((v >> (c - 1)) & 1);
  uint16_t r = c >= 16 ? (uint16_t)((int16_t)v >> (16 - 1))
                       : (uint16_t)((int16_t)v >> c);
  EFLAGS.CF = carry_out;
  EFLAGS.PF = parity_even((uint8_t)r);
  EFLAGS.ZF = r == 0;
  EFLAGS.SF = (int16_t)r < 0;
  if (c == 1) {
    EFLAGS.OF = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86sra.w

### Synopsis

```
Instruction: x86sra.w rj, rk
CPU Flags: LBT
```

### Description

x86-style shift right arithmetic: shift 32-bit value in `rj` right arithmetically by the amount specified in `rk`. If shift count is non-zero, update CF, PF, ZF, SF (OF is set to 0 if count is 1). If shift count is zero, flags are unchanged. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint32_t v = (uint32_t)a;
unsigned c = (unsigned)(b & 0x1f);
if (c != 0) {
  uint8_t carry_out = c > 32 ? ((int32_t)v < 0) : ((v >> (c - 1)) & 1);
  uint32_t r = c >= 32 ? (uint32_t)((int32_t)v >> (32 - 1))
                       : (uint32_t)((int32_t)v >> c);
  EFLAGS.CF = carry_out;
  EFLAGS.PF = parity_even((uint8_t)r);
  EFLAGS.ZF = r == 0;
  EFLAGS.SF = (int32_t)r < 0;
  if (c == 1) {
    EFLAGS.OF = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86sra.d

### Synopsis

```
Instruction: x86sra.d rj, rk
CPU Flags: LBT
```

### Description

x86-style shift right arithmetic: shift 64-bit value in `rj` right arithmetically by the amount specified in `rk`. If shift count is non-zero, update CF, PF, ZF, SF (OF is set to 0 if count is 1). If shift count is zero, flags are unchanged. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint64_t v = (uint64_t)a;
unsigned c = (unsigned)(b & 0x3f);
if (c != 0) {
  uint8_t carry_out = c > 64 ? ((int64_t)v < 0) : ((v >> (c - 1)) & 1);
  uint64_t r = c >= 64 ? (uint64_t)((int64_t)v >> (64 - 1))
                       : (uint64_t)((int64_t)v >> c);
  EFLAGS.CF = carry_out;
  EFLAGS.PF = parity_even((uint8_t)r);
  EFLAGS.ZF = r == 0;
  EFLAGS.SF = (int64_t)r < 0;
  if (c == 1) {
    EFLAGS.OF = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86srai.b

### Synopsis

```
Instruction: x86srai.b rj, imm
CPU Flags: LBT
```

### Description

x86-style shift right arithmetic: shift 8-bit value in `rj` right arithmetically by immediate `imm`. If shift count is non-zero, update CF, PF, ZF, SF (OF is set to 0 if count is 1). If shift count is zero, flags are unchanged. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint8_t v = (uint8_t)a;
unsigned c = (unsigned)imm;
if (c != 0) {
  uint8_t carry_out = c > 8 ? ((int8_t)v < 0) : ((v >> (c - 1)) & 1);
  uint8_t r =
      c >= 8 ? (uint8_t)((int8_t)v >> (8 - 1)) : (uint8_t)((int8_t)v >> c);
  EFLAGS.CF = carry_out;
  EFLAGS.PF = parity_even((uint8_t)r);
  EFLAGS.ZF = r == 0;
  EFLAGS.SF = (int8_t)r < 0;
  if (c == 1) {
    EFLAGS.OF = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86srai.h

### Synopsis

```
Instruction: x86srai.h rj, imm
CPU Flags: LBT
```

### Description

x86-style shift right arithmetic: shift 16-bit value in `rj` right arithmetically by immediate `imm`. If shift count is non-zero, update CF, PF, ZF, SF (OF is set to 0 if count is 1). If shift count is zero, flags are unchanged. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint16_t v = (uint16_t)a;
unsigned c = (unsigned)imm;
if (c != 0) {
  uint8_t carry_out = c > 16 ? ((int16_t)v < 0) : ((v >> (c - 1)) & 1);
  uint16_t r = c >= 16 ? (uint16_t)((int16_t)v >> (16 - 1))
                       : (uint16_t)((int16_t)v >> c);
  EFLAGS.CF = carry_out;
  EFLAGS.PF = parity_even((uint8_t)r);
  EFLAGS.ZF = r == 0;
  EFLAGS.SF = (int16_t)r < 0;
  if (c == 1) {
    EFLAGS.OF = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86srai.w

### Synopsis

```
Instruction: x86srai.w rj, imm
CPU Flags: LBT
```

### Description

x86-style shift right arithmetic: shift 32-bit value in `rj` right arithmetically by immediate `imm`. If shift count is non-zero, update CF, PF, ZF, SF (OF is set to 0 if count is 1). If shift count is zero, flags are unchanged. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint32_t v = (uint32_t)a;
unsigned c = (unsigned)imm;
if (c != 0) {
  uint8_t carry_out = c > 32 ? ((int32_t)v < 0) : ((v >> (c - 1)) & 1);
  uint32_t r = c >= 32 ? (uint32_t)((int32_t)v >> (32 - 1))
                       : (uint32_t)((int32_t)v >> c);
  EFLAGS.CF = carry_out;
  EFLAGS.PF = parity_even((uint8_t)r);
  EFLAGS.ZF = r == 0;
  EFLAGS.SF = (int32_t)r < 0;
  if (c == 1) {
    EFLAGS.OF = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |

## x86srai.d

### Synopsis

```
Instruction: x86srai.d rj, imm
CPU Flags: LBT
```

### Description

x86-style shift right arithmetic: shift 64-bit value in `rj` right arithmetically by immediate `imm`. If shift count is non-zero, update CF, PF, ZF, SF (OF is set to 0 if count is 1). If shift count is zero, flags are unchanged. Only EFLAGS (LBT4) is updated; the GPR is not modified.

### Operation

```
uint64_t v = (uint64_t)a;
unsigned c = (unsigned)imm;
if (c != 0) {
  uint8_t carry_out = c > 64 ? ((int64_t)v < 0) : ((v >> (c - 1)) & 1);
  uint64_t r = c >= 64 ? (uint64_t)((int64_t)v >> (64 - 1))
                       : (uint64_t)((int64_t)v >> c);
  EFLAGS.CF = carry_out;
  EFLAGS.PF = parity_even((uint8_t)r);
  EFLAGS.ZF = r == 0;
  EFLAGS.SF = (int64_t)r < 0;
  if (c == 1) {
    EFLAGS.OF = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 0.5(1/2)         |
| 3C6000 | LA664 | 2       | 0.5(1/2)         |
