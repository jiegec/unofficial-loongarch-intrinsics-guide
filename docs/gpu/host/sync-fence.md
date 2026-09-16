# Synchronization and Traps

| Mechanism | Layer | Contract |
|-----------|-------|----------|
| `POLL` | SCMD32 | bounded register/memory comparison |
| `DBAR` | SCMD32/BPIPE | command-pipe write ordering and EOP visibility |
| `INTR` | SCMD32/BPIPE | pipe completion interrupt |
| `WR32/WR64` | BPIPE | write at EOP |
| `DOORBELL` | HIQ SCMD32 | wake KCD queue by PASID and queue ID |
| KCD debug trap | KCD exception path | wave/debug event attribution |
| shader `check`/`barrier` | shader ISA | shader/workgroup ordering |
| host fence | driver/UAPI | CPU visibility and BO lifetime |

No mechanism substitutes for another. `DOORBELL` does not launch a graphics
IB, shader `exit` does not signal a host fence, and a KCD `rptr` advance is not
evidence of shader execution.

A normal 32-bit GPIPE fence is `VMID(0) | WB32(0) | address64 | sequence32 |
DBAR(0)`. `POLL` waits on a condition with a bounded retry; the kernel
pipeline-sync producer uses equality on memory with mask `0xffffffff`,
interval `1`, and retry count `0xfff`.

Trap events are not completion events. A KCD debug trap requires the
process-debug interface to be enabled and carries process/queue attribution;
the shader `trap` instruction is an L3 operation with its own semantics.
