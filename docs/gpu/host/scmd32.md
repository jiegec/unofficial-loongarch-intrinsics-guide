# Command Words: SCMD32

## The shared L1 word

`SCMD32` is the outer-ring command word. It is used by every L1 ring, but the
vocabulary each pipe consumes is different: a GPIPE ring executes `VMID`,
`IB`, `WB32`, and `POLL`; an XDMA ring executes `STSCFG` and `SBMT`; the CPIPE
HIQ executes `MAP_PROCESS`, `MAP_QUEUE`, and `DOORBELL`. The per-pipe pages
([GPIPE](gpipe.md), [XDMA](xdma.md), [CPIPE](cpipe.md)) define which opcodes
each pipe accepts and what their payloads mean.

```text
31                                                       8 7              0
+---------------------------------------------------------+----------------+
|                     configuration                       |    opcode      |
+---------------------------------------------------------+----------------+
                           24 bits                              8 bits
```

```text
SCMD32(op, cfg) = (op & 0xff) | ((cfg & 0x00ffffff) << 8)
```

## Common payloads

The following payload shapes are shared by several pipes.

### Writeback and events

```text
WB32: [SCMD32(0x10, 0), address_lo, address_hi, data32]
WB64: [SCMD32(0x11, 0), address_lo, address_hi, data_lo, data_hi]
ENVT: [SCMD32(0x12, selector), address_lo, address_hi]
```

The kernel GPIPE/XDMA fence producer uses `WB32`; the BPIPE producer has a
64-bit form. The intended `WB64` packet contains both data words, but a KCD
user-ring opcode `0x11` samples a pipe register in its high word. These are
different front ends.

### Poll

`POLL` packs the condition and memory selector in `cfg`:

```text
dword 0  SCMD32(POLL, condition | (memory << 4))
dword 1  address[31:0]
dword 2  address[63:32]
dword 3  reference
dword 4  mask
dword 5  interval[15:0] | retry_count[31:16]
```

Conditions are `true=0`, `less=1`, `less_equal=2`, `equal=3`, `not_equal=4`,
`greater_equal=5`, and `greater=6`; `memory=1` selects memory. The kernel
pipeline-sync producer uses equality, memory, mask `0xffffffff`, interval `1`,
and retry count `0xfff`.

### Interrupt, barrier, and register write

`INTR` (0x14) is one dword; it interrupts at pipe completion. `DBAR` (0x16) is
one dword; it orders command-processor writes. `WREG` writes a
command-processor register:

```text
WREG: [SCMD32(0x81, 0), register_offset, value]
```

`WREG` targets a command-processor register, not a shader SGPR. Register
offsets outside a documented producer contract must not be guessed.

### VM bind

```text
VM_BIND: [SCMD32(0x84, 0), pipe_type, vmid, pasid]
```

Opcode holes are not automatically reserved; no producer-backed LG200 contract
is assigned to them.
