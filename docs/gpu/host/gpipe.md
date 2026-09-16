# GPIPE

The graphics ring. GPIPE receives `VMID + IB` wrappers and executes them as
inner command streams. It does not decode shader `INST_LO/HI`; it passes stage
`Kernel Entry` addresses after the state is accepted.

## Ring and ownership

| Property | Value |
|----------|-------|
| ICMD MOP | `0x01` (`BGQ`, `GQSZ`, `UBGQ`, `RESET`) |
| Cursors | `0x18` write / `0x1c` read, dwords |
| VMIDs | 1..3 (DRM/GFX); VMID 0 is kernel |
| Completion | IRQ source `0x01` (`CP_END_OF_GPIPE`) |

## Command vocabulary

| Opcode | Payload |
|--------|---------|
| `VMID` (0x0f) | one dword; `cfg[23:0]` is the VMID/domain value |
| `IB` (0x80) | address64 + length/vmid control (16 bytes) |
| `WB32` (0x10) | address64 + data32 |
| `POLL` (0x83) | address64, reference, mask, interval/retry |
| `INTR` (0x14) | interrupt at pipe completion |
| `DBAR` (0x16) | write barrier and ordering |

The outer wrapper is exact:

```text
dword 0  SCMD32(VMID, vmid)
dword 1  SCMD32(IB, 0)
dword 2  ib_gpu_va[31:0]
dword 3  ib_gpu_va[63:32]
dword 4  length_dw[23:0] | vmid[31:24]
```

## Fence

A normal 32-bit GPIPE fence is:

```text
VMID(0) | WB32(0) | address64 | sequence32 | DBAR(0)
```

`DBAR` orders command-processor writes; it is not a shader `check` and not a
CPU fence by itself.
