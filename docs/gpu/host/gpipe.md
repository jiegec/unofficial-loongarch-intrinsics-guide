# GPIPE

GPIPE is the graphics command processor. It consumes the graphics ring,
executes the indirect buffers it is given, and applies the state records that
select the pipeline state. It does not decode shader instructions; it passes the
stage entry addresses to the shader front end after the state is accepted.

## Ring and ownership

| Property | Value |
|----------|-------|
| ICMD block | `0x01` |
| Ring cursors | write `0x18`, read `0x1c` |
| VMIDs | 1–3 |
| Completion | IRQ source `0x01` |

The graphics ring is owned by the kernel. A user-mode driver submits an
indirect buffer and a fence; it does not advance the ring cursor.

## Command vocabulary

A GPIPE ring carries 32-bit stream command words (SCMD32):

```text
31                                                       8 7              0
+---------------------------------------------------------+----------------+
|                     configuration                       |    opcode      |
+---------------------------------------------------------+----------------+
```

```text
SCMD32(op, cfg) = (op & 0xff) | ((cfg & 0x00ff_ffff) << 8)
```

| Opcode | Name | Payload |
|-------:|------|---------|
| `0x00` | `NOP` | None. |
| `0x0f` | `VMID` | One dword: the VMID in the configuration field. |
| `0x80` | `IB` | Five dwords: command, indirect-buffer address low, high, and length with the VMID. |
| `0x10` | `WB32` | Address low, address high, 32-bit data. |
| `0x11` | `WB64` | Address low, address high, data low, data high. |
| `0x12` | `ENVT` | Address low, address high. |
| `0x14` | `INTR` | One dword; interrupt at pipe completion. |
| `0x16` | `DBAR` | One dword; order command-processor writes. |
| `0x81` | `WREG` | Register offset, value. |
| `0x83` | `POLL` | Address low, high, reference, mask, interval and retry. |
| `0x84` | `VM_BIND` | Ring type, VMID, PASID. |

## Indirect buffer

An IB command is:

| Dword | Content |
|------:|---------|
| 0 | `SCMD32(0x0f, vmid)` |
| 1 | `SCMD32(0x80, 0)` |
| 2 | IB address, low 32 bits |
| 3 | IB address, high 32 bits |
| 4 | `length_dwords[23:0]` OR `vmid[31:24]` |

The address MUST be mapped executable. The length is in dwords.

## Fence

A 32-bit fence is the sequence:

```text
SCMD32(0x0f, 0)                  ; select VMID 0
SCMD32(0x10, 0)                  ; WB32
address_low, address_high
sequence_32
SCMD32(0x16, 0)                  ; DBAR
```

A 64-bit fence uses `WB64` and writes two data words. `DBAR` orders the
command-processor writes; it is not a shader ordering instruction and not a CPU
fence.

## VM switch

A VMID's page-directory base is programmed through `WREG`:

| Register | Meaning |
|---------:|---------|
| `0x10020 + 8 * vmid` | Page-directory base, low word. |
| `0x10024 + 8 * vmid` | Page-directory base, high word. |
| `0x10018` | TLB flush control. |

The flush word selects a VMID and whether to flush all entries. A VM switch
MUST program the new base before the flush and MUST NOT reuse a VMID whose
previous work can still be in flight; the kernel enforces the 20 ms drain.

## Poll

`POLL` waits for a memory location to satisfy a condition:

| Field | Meaning |
|-------|---------|
| `condition` | 0 true, 1 less, 2 less-or-equal, 3 equal, 4 not-equal, 5 greater-or-equal, 6 greater. |
| `memory` | 1 selects memory; 0 selects a register. |
| `address` | 64-bit address. |
| `reference` | Comparison value. |
| `mask` | Bit mask applied before the comparison. |
| `interval` | Poll interval. |
| `retry` | Retry count. |

## Ordering and errors

A GPIPE submission completes through its fence. A timeout is not a retry: the
kernel marks the context lost, checks whether a reset occurred, and rebuilds the
context only after a reset is reported.

> UNRESOLVED: the exact behaviour of a fence after a per-pipe reset, and whether
> a partially executed indirect buffer is reported separately from a context
> loss.
