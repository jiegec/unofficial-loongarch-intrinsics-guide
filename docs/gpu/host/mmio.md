# MMIO and Command Words

The kernel drives the command processors through a small register file. A
user-mode driver does not program these registers; the interface is documented
because its state determines what a submission can observe.

## Register map

| Offset | Register |
|-------:|----------|
| `0x00` | Command word. |
| `0x04` | Command status. |
| `0x08`, `0x0c` | Command arguments 0 and 1. |
| `0x10`, `0x14` | Command results 0 and 1. |
| `0x18`, `0x1c` | GPIPE ring write and read cursors. |
| `0x20`, `0x24` | XDMA ring cursors. |
| `0x30`, `0x34` | BPIPE ring cursors. |
| `0x38`, `0x3c` | CPIPE ring cursors. |
| `0x40` | Firmware version. |
| `0x44` | IP state. |
| `0x48`, `0x4c` | DPIPE ring cursors. |
| `0x50`, `0x54` | EPIPE ring cursors. |
| `0x90`, `0x94` | 64-bit time counter, low and high. |
| `0xe8` | DPM state. |

All offsets are in bytes from the register base. A ring cursor is a dword
offset, not a byte offset.

The command status has four defined values:

| Value | Name | Meaning |
|------:|------|---------|
| `0x0000_0000` | NULL | No command. |
| `0xB007_B007` | BOOT | Firmware boot complete. |
| `0xD02E_D02E` | DONE | Command completed. |
| `0xFFFF_0000` | RUN | Command running; the low 16 bits may hold a count. |

## Immediate command word

An immediate command is a 32-bit word:

```text
31                16 15                 8 7                  0
+-------------------+--------------------+--------------------+
|    configuration  |    operation       |        block       |
+-------------------+--------------------+--------------------+
```

The encoding is:

```text
ICMD32(block, operation)      = (block & 0xff) | ((operation & 0xff) << 8)
ICMD32i(block, operation, cfg)= ICMD32(block, operation) | ((cfg & 0xffff) << 16)
```

A command is written to `0x00`; the arguments are written to `0x08` and `0x0c`;
the kernel polls `0x04` for `DONE`. A command MUST NOT be issued while the
previous command is running.

## Blocks

| Block | MOP | Operations |
|-------|----:|------------|
| GPIPE | `0x01` | `BGQ` 1, `GQSZ` 2, `UBGQ` 3, `RESET` 4 |
| BPIPE | `0x02` | `BBQ` 1, `BQSZ` 2, `UBBQ` 3 |
| CPIPE | `0x03` | `BCQ` 1, `CQSZ` 2, `UBCQ` 3, `BCDB` 4, `UBCDB` 5 |
| XDMA | `0x04` | `BDQ` 1, `DQSZ` 2, `UBDQ` 3 |
| SYNC | `0x05` | `GSYNC` 1, `BSYNC` 2, `CSYNC` 3 |
| MMU | `0x06` | `MMUEN` 1, `UDIR` 2, `USAFE` 3, `UPG` 4, `UEXP` 5, `FTLB` 6, `UVMADDR` 7, `UVMSZ` 8, `RETRY` 9 |
| ZIP | `0x07` | `ZEN` 1, `ZDIS` 2, `UTAGADDR` 3, `UTAGMASK` 4, `CNT` 6, `CNTEN` 7, `CNTDIS` 8 |
| FREQ | `0x08` | `UFRQ` 0, `POWER_LEVEL_UFRQ` 1 |
| EXC | `0x09` | `BEQ` 1, `EQSZ` 2, `UBEQ` 3, `HBEQ` 4 |
| DOORBELL | `0x0a` | `DB_ZEN` 1, `DB_ZDIS` 2 |
| CWSR | `0x0b` | `CWSR_ZEN` 1, `CWSR_ZDIS` 2 |
| DPIPE | `0x0c` | `BBQ` 1, `BQSZ` 2, `UBBQ` 3 |
| EPIPE | `0x0d` | `BBQ` 1, `BQSZ` 2, `UBBQ` 3 |
| VRAM | `0x0e` | `BINDADDR` 1, `READ` 2, `WRITE` 3 |

The operation names are the kernel's names. A user-mode driver never emits an
immediate command.

## Ring provisioning

A ring is a circular buffer of command words in GPU memory. Provisioning a ring
takes three steps: unmap the previous queue, bind the new queue, and set its
size.

For GPIPE the sequence is:

1. `UBGQ` with the old queue address;
2. `BGQ` with the new queue address;
3. `GQSZ` with the ring size in dwords.

A ring's write cursor is advanced by the producer; the command processor
consumes words up to the read cursor. The two cursors are independent, and a
producer MUST NOT overwrite a word that the command processor has not consumed.

## Interrupts and time

End-of-pipe interrupts are identified by a source id:

| Source | ID |
|--------|----|
| GPIPE end of pipe | `0x01` |
| BPIPE end of pipe | `0x02` |
| CPIPE | `0x22` |
| XDMA | `0x23` |

The 64-bit time counter at `0x90`/`0x94` is a free-running device clock. Its
unit is the device clock; a driver MUST calibrate it against a host clock before
using it for wall-clock timing.

## Reset

A pipe reset is an immediate command (`GPIPE RESET`, and the corresponding
commands for the other pipes). A reset invalidates the pipe's queued work and
starts a new epoch. After a reset, the kernel reprograms the ring and the page
tables before admitting new work.

> UNRESOLVED: the exact state preserved by a per-pipe reset, and whether a
> CPIPE reset affects an in-flight user queue.

## Open questions

- The semantics of the `ZIP`, `EXC`, `CWSR` and `VRAM` operations beyond their
  names.
- Whether the `IP_STATE` register reports a per-pipe or a global state.
- The unit and wrap behaviour of the time counter.
