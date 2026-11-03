# CPIPE

CPIPE is the compute command processor. Compute work is queue-based: the kernel
creates a queue, the queue's user ring carries the command grammar, and the
user-mode driver publishes a write pointer and rings a doorbell. CPIPE consumes
the queue and launches the compute kernel.

## Ring and ownership

| Property | Value |
|----------|-------|
| ICMD block | `0x03` |
| Ring cursors | write `0x38`, read `0x3c` |
| VMIDs | 4–7 |
| Queue ids | 0–7 per PASID |
| Completion | IRQ source `0x22` |

The management ring is owned by the kernel. A user-mode driver owns a user
queue ring and its 64-byte index block; it does not program the CPIPE ring
cursor.

## Management packets

The kernel writes a dword stream to the management ring. Each packet starts
with a stream command word.

### MAP_PROCESS

| Word | Content |
|-----:|---------|
| 0 | `SCMD32(0xe0, 0)` |
| 1 | PASID |
| 2 | Reserved, zero |
| 3–4 | Page-directory base, 64 bits |
| 5–6 | Trap base address, 64 bits |
| 7–8 | Trap memory address, 64 bits |
| 9 | Debug flags |

### MAP_QUEUE

| Word | Content |
|-----:|---------|
| 0 | `SCMD32(0xe2, 0)` |
| 1–14 | The 14-dword memory queue descriptor |

### UNMAP_QUEUE

| Word | Content |
|-----:|---------|
| 0 | `SCMD32(0xe3, 0)` |
| 1 | Filter: 1 all queues, 2 dynamic, 3 by PASID |
| 2 | Filter data |

### DOORBELL

| Word | Content |
|-----:|---------|
| 0 | `SCMD32(0xe4, 0)` |
| 1 | PASID |
| 2 | Queue id |

### QUERY_STATUS

A five-dword writeback into the queue's completion word. The kernel uses it to
sample the queue's status without a separate register read.

## Memory queue descriptor

The MQD is 14 dwords:

| Dword | Field | Meaning |
|------:|-------|---------|
| 0–1 | User-ring base | 64-bit GPU virtual address. |
| 2–3 | Read pointer address | 64-bit address of the read index. |
| 4–5 | Write pointer address | 64-bit address of the write index. |
| 6 | Ring length | In dwords. |
| 7 | PASID | Process address-space identifier. |
| 8 | Queue id | 0–7. |
| 9 | Queue type and active | Type in `[3:0]`, active in `[4]`. |
| 10 | Doorbell offset | Zero on LG200. |
| 11–12 | CWSR address | Zero on the LG200 path. |
| 13 | CWSR size | Zero on the LG200 path. |

Queue types are compute (0), XDMA (1) and high priority (2). The active bit is
set for a queue that can run.

> UNRESOLVED: whether LG200 supports the CWSR fields at all. The current path
> leaves them zero and does not expose a context-save interface.

## User queue

A user queue is created with `USERQ`. The queue ring is a power-of-two buffer
from 4 KiB to 1 MiB. A 64-byte index block precedes the ring:

| Offset | Field |
|--------|-------|
| `queue_va - 64` | 64-bit write index, in bytes. |
| `queue_va - 56` | 64-bit read index, in bytes. |
| `queue_va - 48` | Reserved. |

The driver writes complete packets into the ring, publishes the new write
pointer by store, and rings the doorbell. The kernel forwards the doorbell to
the firmware through the HIQ `DOORBELL` packet. The kernel does not parse the
ring.

## Completion

A compute queue completes through the CPIPE end-of-pipe interrupt and the
submission fence. The fence is the user-visible completion; the interrupt is the
kernel's signal.

## Open questions

- The exact doorbell acknowledgement and the queue's read-index update.
- The CWSR contract, if one exists on LG200.
- The interaction between a CPIPE reset and a queue that is still mapped.
