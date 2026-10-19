# LBT Integer

## addu12i.w

### Synopsis

```
Instruction: addu12i.w rd, rj, imm
CPU Flags: LBT
```

### Description

Add sign-extended 5-bit immediate left-shifted by 12 to unsigned 32-bit value in `rj`, sign-extend the 32-bit result to 64-bit and store in `rd`.

### Operation

```
int64_t offset = (int64_t)sext(imm & 0x1f, 5) << 12;
dst = sext((uint32_t)a + (uint32_t)offset, 32);
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## addu12i.d

### Synopsis

```
Instruction: addu12i.d rd, rj, imm
CPU Flags: LBT
```

### Description

Add sign-extended 5-bit immediate left-shifted by 12 to 64-bit value in `rj` and store the result in `rd`.

### Operation

```
int64_t offset = (int64_t)sext(imm & 0x1f, 5) << 12;
dst = a + (uint64_t)offset;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## adc.b

### Synopsis

```
Instruction: adc.b rd, rj, rk
CPU Flags: LBT
```

### Description

Add 8-bit values in `rj` and `rk` with carry (CF in EFLAGS), sign-extend the result to 64-bit and store in `rd`.

### Operation

```
uint8_t lhs = a;
uint8_t rhs = b;
uint8_t cf = EFLAGS.CF;
uint8_t r = lhs + rhs + cf;
dst = sext(r, 8);
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 1                |
| 3C6000 | LA664 | 2       | 1                |

## adc.h

### Synopsis

```
Instruction: adc.h rd, rj, rk
CPU Flags: LBT
```

### Description

Add 16-bit values in `rj` and `rk` with carry (CF in EFLAGS), sign-extend the result to 64-bit and store in `rd`.

### Operation

```
uint16_t lhs = a;
uint16_t rhs = b;
uint8_t cf = EFLAGS.CF;
uint16_t r = lhs + rhs + cf;
dst = sext(r, 16);
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 1                |
| 3C6000 | LA664 | 2       | 1                |

## adc.w

### Synopsis

```
Instruction: adc.w rd, rj, rk
CPU Flags: LBT
```

### Description

Add 32-bit values in `rj` and `rk` with carry (CF in EFLAGS), sign-extend the result to 64-bit and store in `rd`.

### Operation

```
uint32_t lhs = a;
uint32_t rhs = b;
uint8_t cf = EFLAGS.CF;
uint32_t r = lhs + rhs + cf;
dst = sext(r, 32);
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 1                |
| 3C6000 | LA664 | 2       | 1                |

## adc.d

### Synopsis

```
Instruction: adc.d rd, rj, rk
CPU Flags: LBT
```

### Description

Add 64-bit values in `rj` and `rk` with carry (CF in EFLAGS), sign-extend the result to 64-bit and store in `rd`.

### Operation

```
uint64_t lhs = a;
uint64_t rhs = b;
uint8_t cf = EFLAGS.CF;
uint64_t r = lhs + rhs + cf;
dst = r;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 1                |
| 3C6000 | LA664 | 2       | 1                |

## sbc.b

### Synopsis

```
Instruction: sbc.b rd, rj, rk
CPU Flags: LBT
```

### Description

Subtract 8-bit values in `rj` and `rk` with borrow (CF in EFLAGS), sign-extend the result to 64-bit and store in `rd`.

### Operation

```
uint8_t lhs = a;
uint8_t rhs = b;
uint8_t cf = EFLAGS.CF;
uint8_t r = lhs - rhs - cf;
dst = sext(r, 8);
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 1                |
| 3C6000 | LA664 | 2       | 1                |

## sbc.h

### Synopsis

```
Instruction: sbc.h rd, rj, rk
CPU Flags: LBT
```

### Description

Subtract 16-bit values in `rj` and `rk` with borrow (CF in EFLAGS), sign-extend the result to 64-bit and store in `rd`.

### Operation

```
uint16_t lhs = a;
uint16_t rhs = b;
uint8_t cf = EFLAGS.CF;
uint16_t r = lhs - rhs - cf;
dst = sext(r, 16);
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 1                |
| 3C6000 | LA664 | 2       | 1                |

## sbc.w

### Synopsis

```
Instruction: sbc.w rd, rj, rk
CPU Flags: LBT
```

### Description

Subtract 32-bit values in `rj` and `rk` with borrow (CF in EFLAGS), sign-extend the result to 64-bit and store in `rd`.

### Operation

```
uint32_t lhs = a;
uint32_t rhs = b;
uint8_t cf = EFLAGS.CF;
uint32_t r = lhs - rhs - cf;
dst = sext(r, 32);
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 1                |
| 3C6000 | LA664 | 2       | 1                |

## sbc.d

### Synopsis

```
Instruction: sbc.d rd, rj, rk
CPU Flags: LBT
```

### Description

Subtract 64-bit values in `rj` and `rk` with borrow (CF in EFLAGS), sign-extend the result to 64-bit and store in `rd`.

### Operation

```
uint64_t lhs = a;
uint64_t rhs = b;
uint8_t cf = EFLAGS.CF;
uint64_t r = lhs - rhs - cf;
dst = r;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 1                |
| 3C6000 | LA664 | 2       | 1                |

## rotr.b

### Synopsis

```
Instruction: rotr.b rd, rj, rk
CPU Flags: LBT
```

### Description

Rotate 8-bit value in `rj` right by the amount specified in `rk`. The shift amount is masked modulo 8.

### Operation

```
uint8_t v = (uint8_t)a;
unsigned c = (unsigned)b % 8;
uint8_t r = c == 0 ? v : (uint8_t)((v >> c) | (v << (8 - c)));
dst = sext(r, 8);
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 1                |
| 3C6000 | LA664 | 2       | 1                |

## rotr.h

### Synopsis

```
Instruction: rotr.h rd, rj, rk
CPU Flags: LBT
```

### Description

Rotate 16-bit value in `rj` right by the amount specified in `rk`. The shift amount is masked modulo 16.

### Operation

```
uint16_t v = (uint16_t)a;
unsigned c = (unsigned)b % 16;
uint16_t r = c == 0 ? v : (uint16_t)((v >> c) | (v << (16 - c)));
dst = sext(r, 16);
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 1                |
| 3C6000 | LA664 | 2       | 1                |

## rotr.w

### Synopsis

```
Instruction: rotr.w rd, rj, rk
CPU Flags: LBT
```

### Description

Rotate 32-bit value in `rj` right by the amount specified in `rk`. The shift amount is masked modulo 32.

### Operation

```
uint32_t v = (uint32_t)a;
unsigned c = (unsigned)b % 32;
uint32_t r = c == 0 ? v : (uint32_t)((v >> c) | (v << (32 - c)));
dst = sext(r, 32);
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## rotr.d

### Synopsis

```
Instruction: rotr.d rd, rj, rk
CPU Flags: LBT
```

### Description

Rotate 64-bit value in `rj` right by the amount specified in `rk`. The shift amount is masked modulo 64.

### Operation

```
uint64_t v = (uint64_t)a;
unsigned c = (unsigned)b % 64;
uint64_t r = c == 0 ? v : (uint64_t)((v >> c) | (v << (64 - c)));
dst = r;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## rotri.b

### Synopsis

```
Instruction: rotri.b rd, rj, imm
CPU Flags: LBT
```

### Description

Rotate 8-bit value in `rj` right by immediate `imm`. The shift amount is masked modulo 8.

### Operation

```
uint8_t v = (uint8_t)a;
unsigned c = imm % 8;
uint8_t r = c == 0 ? v : (uint8_t)((v >> c) | (v << (8 - c)));
dst = sext(r, 8);
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 1                |
| 3C6000 | LA664 | 2       | 1                |

## rotri.h

### Synopsis

```
Instruction: rotri.h rd, rj, imm
CPU Flags: LBT
```

### Description

Rotate 16-bit value in `rj` right by immediate `imm`. The shift amount is masked modulo 16.

### Operation

```
uint16_t v = (uint16_t)a;
unsigned c = imm % 16;
uint16_t r = c == 0 ? v : (uint16_t)((v >> c) | (v << (16 - c)));
dst = sext(r, 16);
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 1                |
| 3C6000 | LA664 | 2       | 1                |

## rotri.w

### Synopsis

```
Instruction: rotri.w rd, rj, imm
CPU Flags: LBT
```

### Description

Rotate 32-bit value in `rj` right by immediate `imm`. The shift amount is masked modulo 32.

### Operation

```
uint32_t v = (uint32_t)a;
unsigned c = imm % 32;
uint32_t r = c == 0 ? v : (uint32_t)((v >> c) | (v << (32 - c)));
dst = sext(r, 32);
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## rotri.d

### Synopsis

```
Instruction: rotri.d rd, rj, imm
CPU Flags: LBT
```

### Description

Rotate 64-bit value in `rj` right by immediate `imm`. The shift amount is masked modulo 64.

### Operation

```
uint64_t v = (uint64_t)a;
unsigned c = imm % 64;
uint64_t r = c == 0 ? v : (uint64_t)((v >> c) | (v << (64 - c)));
dst = r;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## rcr.b

### Synopsis

```
Instruction: rcr.b rd, rj, rk
CPU Flags: LBT
```

### Description

Rotate 8-bit value in `rj` and CF (in EFLAGS) together as a 9-bit ring right by the amount specified in `rk`. The result is written to `rd`.

### Operation

```
uint8_t v = a;
// Mask count to 5 bits (matching x86 CL masking), then modulo (bits+1)
// for the 9-bit [CF, v] ring rotation.
unsigned c = ((unsigned)b & 0x1F) % (8 + 1);
// Rotate [CF, v] right by c, write low 8 bits to rd
uint64_t ring_hi = EFLAGS.CF;
uint64_t ring_lo = v;
uint64_t r;
if (c == 0) {
  r = v;
} else {
  uint64_t tmp_hi = ring_hi, tmp_lo = ring_lo;
  for (unsigned i = 0; i < c; i++) {
    uint64_t new_lo = (tmp_lo >> 1) | (tmp_hi << (8 - 1));
    uint64_t new_hi = tmp_lo & 1;
    tmp_lo = new_lo;
    tmp_hi = new_hi;
  }
  r = tmp_lo & ((1ULL << 8) - 1);
}
dst = sext(r, 8);
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 1                |
| 3C6000 | LA664 | 2       | 1                |

## rcr.h

### Synopsis

```
Instruction: rcr.h rd, rj, rk
CPU Flags: LBT
```

### Description

Rotate 16-bit value in `rj` and CF (in EFLAGS) together as a 17-bit ring right by the amount specified in `rk`. The result is written to `rd`.

### Operation

```
uint16_t v = (uint16_t)a;
unsigned c = ((unsigned)b & 0x1F) % (16 + 1);
// Rotate [CF, v] right by c, write low 16 bits to rd
uint64_t ring_hi = EFLAGS.CF;
uint64_t ring_lo = v;
uint64_t r;
if (c == 0) {
  r = v;
} else {
  uint64_t tmp_hi = ring_hi, tmp_lo = ring_lo;
  for (unsigned i = 0; i < c; i++) {
    uint64_t new_lo = (tmp_lo >> 1) | (tmp_hi << (16 - 1));
    uint64_t new_hi = tmp_lo & 1;
    tmp_lo = new_lo;
    tmp_hi = new_hi;
  }
  r = tmp_lo & ((1ULL << 16) - 1);
}
dst = sext(r, 16);
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 1                |
| 3C6000 | LA664 | 2       | 1                |

## rcr.w

### Synopsis

```
Instruction: rcr.w rd, rj, rk
CPU Flags: LBT
```

### Description

Rotate 32-bit value in `rj` and CF (in EFLAGS) together as a 33-bit ring right by the amount specified in `rk`. The result is written to `rd`.

### Operation

```
uint32_t v = (uint32_t)a;
unsigned c = (unsigned)b & 0x1f;
// Rotate [CF, v] right by c, write low 32 bits to rd
uint64_t ring_hi = EFLAGS.CF;
uint64_t ring_lo = v;
uint64_t r;
if (c == 0) {
  r = v;
} else {
  uint64_t tmp_hi = ring_hi, tmp_lo = ring_lo;
  for (unsigned i = 0; i < c; i++) {
    uint64_t new_lo = (tmp_lo >> 1) | (tmp_hi << (32 - 1));
    uint64_t new_hi = tmp_lo & 1;
    tmp_lo = new_lo;
    tmp_hi = new_hi;
  }
  r = tmp_lo & ((1ULL << 32) - 1);
}
dst = sext(r, 32);
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 1                |
| 3C6000 | LA664 | 2       | 1                |

## rcr.d

### Synopsis

```
Instruction: rcr.d rd, rj, rk
CPU Flags: LBT
```

### Description

Rotate 64-bit value in `rj` and CF (in EFLAGS) together as a 65-bit ring right by the amount specified in `rk`. The result is written to `rd`.

### Operation

```
uint64_t v = (uint64_t)a;
unsigned c = (unsigned)b & 0x3f;
// Rotate [CF, v] right by c, write low 64 bits to rd
uint64_t ring_hi = EFLAGS.CF;
uint64_t ring_lo = v;
uint64_t r;
if (c == 0) {
  r = v;
} else {
  uint64_t tmp_hi = ring_hi, tmp_lo = ring_lo;
  for (unsigned i = 0; i < c; i++) {
    uint64_t new_lo = (tmp_lo >> 1) | (tmp_hi << (64 - 1));
    uint64_t new_hi = tmp_lo & 1;
    tmp_lo = new_lo;
    tmp_hi = new_hi;
  }
  r = tmp_lo;
}
dst = r;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 1                |
| 3C6000 | LA664 | 2       | 1                |

## rcri.b

### Synopsis

```
Instruction: rcri.b rd, rj, imm
CPU Flags: LBT
```

### Description

Rotate 8-bit value in `rj` and CF (in EFLAGS) together as a 9-bit ring right by immediate `imm`. The result is written to `rd`.

### Operation

```
uint8_t v = (uint8_t)a;
unsigned c = (unsigned)imm % (8 + 1);
// Rotate [CF, v] right by c, write low 8 bits to rd
uint64_t ring_hi = EFLAGS.CF;
uint64_t ring_lo = v;
uint64_t r;
if (c == 0) {
  r = v;
} else {
  uint64_t tmp_hi = ring_hi, tmp_lo = ring_lo;
  for (unsigned i = 0; i < c; i++) {
    uint64_t new_lo = (tmp_lo >> 1) | (tmp_hi << (8 - 1));
    uint64_t new_hi = tmp_lo & 1;
    tmp_lo = new_lo;
    tmp_hi = new_hi;
  }
  r = tmp_lo & ((1ULL << 8) - 1);
}
dst = sext(r, 8);
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 1                |
| 3C6000 | LA664 | 2       | 1                |

## rcri.h

### Synopsis

```
Instruction: rcri.h rd, rj, imm
CPU Flags: LBT
```

### Description

Rotate 16-bit value in `rj` and CF (in EFLAGS) together as a 17-bit ring right by immediate `imm`. The result is written to `rd`.

### Operation

```
uint16_t v = (uint16_t)a;
unsigned c = (unsigned)imm % (16 + 1);
// Rotate [CF, v] right by c, write low 16 bits to rd
uint64_t ring_hi = EFLAGS.CF;
uint64_t ring_lo = v;
uint64_t r;
if (c == 0) {
  r = v;
} else {
  uint64_t tmp_hi = ring_hi, tmp_lo = ring_lo;
  for (unsigned i = 0; i < c; i++) {
    uint64_t new_lo = (tmp_lo >> 1) | (tmp_hi << (16 - 1));
    uint64_t new_hi = tmp_lo & 1;
    tmp_lo = new_lo;
    tmp_hi = new_hi;
  }
  r = tmp_lo & ((1ULL << 16) - 1);
}
dst = sext(r, 16);
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 1                |
| 3C6000 | LA664 | 2       | 1                |

## rcri.w

### Synopsis

```
Instruction: rcri.w rd, rj, imm
CPU Flags: LBT
```

### Description

Rotate 32-bit value in `rj` and CF (in EFLAGS) together as a 33-bit ring right by immediate `imm`. The result is written to `rd`.

### Operation

```
uint32_t v = (uint32_t)a;
unsigned c = (unsigned)imm % (32 + 1);
// Rotate [CF, v] right by c, write low 32 bits to rd
uint64_t ring_hi = EFLAGS.CF;
uint64_t ring_lo = v;
uint64_t r;
if (c == 0) {
  r = v;
} else {
  uint64_t tmp_hi = ring_hi, tmp_lo = ring_lo;
  for (unsigned i = 0; i < c; i++) {
    uint64_t new_lo = (tmp_lo >> 1) | (tmp_hi << (32 - 1));
    uint64_t new_hi = tmp_lo & 1;
    tmp_lo = new_lo;
    tmp_hi = new_hi;
  }
  r = tmp_lo & ((1ULL << 32) - 1);
}
dst = sext(r, 32);
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 1                |
| 3C6000 | LA664 | 2       | 1                |

## rcri.d

### Synopsis

```
Instruction: rcri.d rd, rj, imm
CPU Flags: LBT
```

### Description

Rotate 64-bit value in `rj` and CF (in EFLAGS) together as a 65-bit ring right by immediate `imm`. The result is written to `rd`.

### Operation

```
uint64_t v = (uint64_t)a;
unsigned c = (unsigned)imm % (64 + 1);
// Rotate [CF, v] right by c, write low 64 bits to rd
uint64_t ring_hi = EFLAGS.CF;
uint64_t ring_lo = v;
uint64_t r;
if (c == 0) {
  r = v;
} else {
  uint64_t tmp_hi = ring_hi, tmp_lo = ring_lo;
  for (unsigned i = 0; i < c; i++) {
    uint64_t new_lo = (tmp_lo >> 1) | (tmp_hi << (64 - 1));
    uint64_t new_hi = tmp_lo & 1;
    tmp_lo = new_lo;
    tmp_hi = new_hi;
  }
  r = tmp_lo;
}
dst = r;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 1                |
| 3C6000 | LA664 | 2       | 1                |
