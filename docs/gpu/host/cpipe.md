# CPIPE

The compute pipe. LG200 compute queues are owned by KCD: the kernel creates a
queue (MQD), the queue's user ring carries the compute command grammar, and
submission happens through the HIQ `DOORBELL`. There is no user-visible BAR
doorbell on LG200.

## Ring and ownership

| Property | Value |
|----------|-------|
| ICMD MOP | `0x03` (`BCQ`, `CQSZ`, `UBCQ`, `BCDB`, `UBCDB`) |
| HIQ cursors | `0x38` write / `0x3c` read, dwords |
| VMIDs | 4..7 (KCD); queues 0..7 |
| Completion | IRQ source `0x22` (`CPIPE`) |

## HIQ management packets

The kernel writes a dword-based HIQ stream:

| Packet | Layout |
|--------|--------|
| `MAP_PROCESS (0xe0)` | command, PASID, reserved, page-table base64, trap-base64, trap-memory64, debug flags |
| `MAP_QUEUE (0xe2)` | command plus 14-dword MQD |
| `UNMAP_QUEUE (0xe3)` | command, filter, data |
| `DOORBELL (0xe4)` | command, PASID, queue ID |
| `WB64 (0x11)` | management status query/fence shape |

The management payloads are fixed-width:

```text
MAP_PROCESS:
  dword 0     SCMD32(MAP_PROCESS, 0)
  dword 1     PASID
  dword 2     reserved (zero)
  dword 3..4  page-table base address64
  dword 5..6  trap base address64
  dword 7..8  trap memory address64
  dword 9     debug flags

UNMAP_QUEUE:
  dword 0     SCMD32(UNMAP_QUEUE, 0)
  dword 1     filter (1=all, 2=dynamic, 3=by PASID)
  dword 2     filter data

DOORBELL:
  dword 0     SCMD32(DOORBELL, 0)
  dword 1     PASID
  dword 2     queue ID
```

The LG200 kernel route leaves the LG210 trap/CWSR extensions disabled where
the device does not advertise them. The fields remain typed positions in the
packet; a zero value is not a general proof that the hardware ignores them.

## The 14-dword MQD

```text
dword 0..1   user-ring base address64
dword 2..3   memory rptr address64
dword 4..5   memory wptr address64
dword 6      ring length in dwords
dword 7      PASID
dword 8      queue ID
dword 9      queue type[3:0] | active[4]
dword 10     doorbell offset
dword 11..12 CWSR address64
dword 13     CWSR size
```

Queue types are `COMPUTE=0`, `XDMA=1`, and `HIGH_PRIORITY=2`. LG200 exposes
queue IDs 0--7 for KCD CPIPE. CWSR and hardware doorbell-base fields are
LG210-only in the current LG200 route.

## User ring and submission

The MQD length is in dwords, but a KCD user queue publishes byte `rptr/wptr`
values. A producer must release-store a byte offset and submit through the HIQ
`DOORBELL`. The user-ring front end is distinct from GPIPE. `WB32`, `POLL`,
and status forms are observable control commands; the wave-launch payload and
completion contract remain unresolved.

## DPIPE and EPIPE

DPIPE and EPIPE have ICMD setup and ring cursor registers in the shared LG2XX
interface, but their completion sources and queue paths are LG210-only in the
current source. LG200 has no developer-facing payload schema for these pipes;
they are reserved routes, not BPIPE aliases.
